export module Vk.HostAllocator;

import Vulkan;
import <memory>;
import <new>;
import Vk.LogicalDevice;
import Vk.PhysicalDevice;
import Vk.VertexBuffer;
import Vk.Memory;

export namespace Alloc {
struct HostAllocatorRequirement {
    HostAllocatorRequirement(LogicalDevice _logical, PhysicalDevice _physical) {
        static LogicalDevice  logical = _logical;
        static PhysicalDevice physical = _physical;
    }

    static inline LogicalDevice  logical  = LogicalDevice::invalid();
    static inline PhysicalDevice physical = PhysicalDevice::invalid();
}; // HostAllocatorRequirement

template < typename Tp >
struct HostAllocator {
    typedef Tp value_type;

    VertexBuffer        host;
    
    HostAllocator() noexcept;
    ~HostAllocator() noexcept;

    template<typename U> bool
    operator==(const HostAllocator<U>&) const noexcept { return true; }

    template<typename U> bool
    operator!=(const HostAllocator<U>&) const noexcept { return false; }

    Tp* allocate(const size_t n);
    void deallocate(Tp* const p, size_t) const noexcept;
};

template < typename T > 
HostAllocator<T>::HostAllocator() noexcept
        : host(HostAllocatorRequirement::logical) {
    std::cout << HostAllocatorRequirement::logical << std::endl;
}

template < typename T > 
HostAllocator<T>::~HostAllocator() noexcept {
    // vkUnmapMemory (
    //     HostAllocatorRequirement::logical,
    //     host._mem
    // );
}

template < typename T > T* 
HostAllocator<T>::allocate(const size_t n) {
    if (n == 0)
    {
        return nullptr;
    }
    if (n > static_cast<size_t>(-1) / sizeof(T))
    {
        throw std::bad_array_new_length();
    }
    
    void* pv =nullptr;
    
    host.gen_staging_buff (
        HostAllocatorRequirement::logical,
        HostAllocatorRequirement::physical,
        n * sizeof(T)
    );

    vkMapMemory (
        HostAllocatorRequirement::logical,
        host._mem, 0, n, 0, &pv
    );

    if (!pv) { throw std::bad_alloc(); }
    return static_cast<T*>(pv);
}

template < typename T > void
HostAllocator<T>::deallocate(T * const p, size_t) const noexcept {
}

} // export
