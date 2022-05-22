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
    static inline LogicalDevice  logical  = LogicalDevice::invalid();
    static inline PhysicalDevice physical = PhysicalDevice::invalid();
}; // HostAllocatorRequirement

struct MapMemory {
    Memory& _mem;

    ~MapMemory();
    MapMemory(Memory& mem, size_t offset, size_t size, void** pv);

    void unmap();
};

MapMemory::MapMemory(Memory& mem, size_t offset, size_t size, void** pv):
        _mem(mem) {
    vkMapMemory (
        HostAllocatorRequirement::logical,
        _mem, offset, size, 0, pv
    );
}

MapMemory::~MapMemory() {
    unmap();
}

void MapMemory::unmap() {
    vkUnmapMemory(HostAllocatorRequirement::logical, _mem);
}

template < typename Tp >
struct HostAllocator {
    typedef Tp value_type;

    StagingBuffer           host;
    void*                   begin;                   
    
    HostAllocator() noexcept;
    ~HostAllocator() noexcept;

    template<typename U> bool
    operator==(const HostAllocator<U>&) const noexcept { return true; }

    template<typename U> bool
    operator!=(const HostAllocator<U>&) const noexcept { return false; }

    Tp* allocate(const size_t n);
    void deallocate(Tp* const p, size_t) const noexcept;
    void construct(Tp* p, const Tp& val);

    template < typename... Args>
    void construct(Tp* p, Args&&... args);


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
    if (n == 0) {
        return nullptr;
    } if (n > static_cast<size_t>(-1) / sizeof(T)) {
        throw std::bad_array_new_length();
    }
    
    host.allocate (
        HostAllocatorRequirement::logical,
        HostAllocatorRequirement::physical,
        n * sizeof(T)
    );

    MapMemory guard{host._mem, 0, n, &begin};

    if (!begin) { throw std::bad_alloc(); }
    return static_cast<T*>(begin);
}

template < typename T > void
HostAllocator<T>::deallocate(T * const p, size_t) const noexcept {
}

template < typename Tp > void
HostAllocator<Tp>::construct(Tp* p, const Tp& val) {
    MapMemory guard(host._mem, (p - (Tp*)begin)/sizeof(Tp), 1, &begin);
    ::new((void *)p) Tp{val};
}

template < typename Tp >
    template < typename... Args> void
HostAllocator<Tp>::construct(Tp* p, Args&&... args) {
    MapMemory guard(host._mem, (p - (Tp*)begin)/sizeof(Tp), 1, &begin);
    std::construct_at(p, args...);
}


} // export
