/**
 * @file VkObject.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides abstraction above Vulkan object
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module App.NativeWrapper;

export import <memory>;

import Vulkan;

export template < 
    typename NativeHandle,
    typename AbstractType
> class NativeWrapper 
    // protected std::enable_shared_from_this < AbstractType > 
    {
public:
    using pointer         = std::shared_ptr < AbstractType >;
    using reference       = std::weak_ptr < AbstractType >;
    using const_pointer   = std::shared_ptr < const AbstractType >;
    using const_reference = std::weak_ptr < const AbstractType >;

protected:
    NativeHandle                _native;

protected:
    NativeWrapper() =default;
    ~NativeWrapper() =default;

    // NativeWrapper(NativeHandle native): _native(native) {  }

public:
    NativeWrapper(const NativeWrapper&) =delete;
    NativeWrapper& operator =(const NativeWrapper&) =delete;

    NativeWrapper(NativeWrapper&&) noexcept =default;
    NativeWrapper& operator =(NativeWrapper&&) noexcept =default;
    
    operator NativeHandle() noexcept {
        return get();
    }

    operator const NativeHandle() const noexcept {
        return get();
    }

    NativeHandle& get() noexcept {
        return _native;
    }

    const NativeHandle& get() const noexcept {
        return _native;
    }

    // pointer ptr() {
    //     return ref().lock();
    // }

    // reference ref() noexcept {
    //     return static_cast<AbstractType*>(this)->weak_from_this();
    // }

    // const_pointer cptr() const {
    //     return cref().lock();
    // }

    // const_reference cref() const noexcept {
    //     return static_cast<const AbstractType*>(this)->weak_from_this();
    // }

};