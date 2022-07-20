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

import <memory>;
import <iostream>;

import Vulkan;

export namespace vk {
enum class StorePolitics {
    Direct,
    Shared,
}; // StorePolitics

inline constexpr StorePolitics default_store_politics = StorePolitics::Direct;

template <
    typename T, 
    StorePolitics pol
> struct WrapedTypeImpl;

template <typename T>
struct WrapedTypeImpl < T, StorePolitics::Direct> {
    using type = T;    
}; // WrapedTypeImpl

template <typename T>
struct WrapedTypeImpl < T, StorePolitics::Shared> {
    using type = std::shared_ptr<T>;
}; // WrapedTypeImpl

template <
    typename T, 
    StorePolitics pol
> using WrapedType = typename WrapedTypeImpl<T, pol>::type;

template < typename Native >
struct NativeDeleter {
    virtual void     increase()                     noexcept =0;
    virtual void     decrease()                     noexcept =0;
    virtual uint32_t count()                        noexcept =0;
    virtual void     release(const Native& n) const noexcept =0;
    virtual         ~NativeDeleter() {  }
}; // NativeDeleter

template <
    typename Native,
    typename Deleter
> struct NativeDeleterImpl: public NativeDeleter < Native > {
    NativeDeleterImpl(Deleter&& del): 
        _counter(1),
        _del(std::forward<Deleter>(del)) {  }
    virtual void release(const Native& n) const noexcept final {
        if(!_counter) std::invoke(_del, n);
    }    
    virtual void     increase() noexcept final { ++_counter; }   
    virtual void     decrease() noexcept final { --_counter; }
    virtual uint32_t count()    noexcept final { return _counter; }

    uint32_t    _counter =0;
    Deleter     _del;
}; // NativeDeleterImpl

template <
    typename Native
> struct EmptyNativeDeleterImpl: public NativeDeleter < Native > {
    EmptyNativeDeleterImpl() {  }
    virtual void release([[maybe_unused]] const Native& n) const noexcept final {  }
    virtual void     increase() noexcept final {  }   
    virtual void     decrease() noexcept final {  }
    virtual uint32_t count()    noexcept final { return 1; }
}; // EmptyNativeDeleterImpl

template < typename Native >
class DeleterGuard {
private:
    using Deleter  = NativeDeleter < Native >*;
    
    Deleter                     _del;

public:
    DeleterGuard(Deleter&& del = new EmptyNativeDeleterImpl<Native>{}):
        _del(std::move(del)) {  }

    DeleterGuard(const DeleterGuard& oth):
        _del(oth._del) { _del->increase(); }

    DeleterGuard& operator =(const DeleterGuard& oth) {
        _del = oth._del;
        _del->increase();
        return *this;
    }
    
    DeleterGuard(DeleterGuard&&) =default;
    DeleterGuard& operator =(DeleterGuard&&) =default;

    void release(const Native& n) {
        _del->decrease();
        _del->release(n);
    }

    ~DeleterGuard() {
        if(!_del->count()) delete _del;
    }
};

template <
    typename        Native,
    StorePolitics   pol =default_store_politics
> class NativeWrapper {
protected:
    using value_type = Native;
    using Internal   = NativeWrapper < value_type, pol >;
    using Wrapped    = WrapedType < value_type, pol >;
    using Deleter    = DeleterGuard < value_type >;

public:
    // using const_pointer = std::shared_ptr<Internal>;

protected:
    Wrapped                               _native;
    Deleter                               _del;

public:
    // template <typename D>
    // NativeWrapper(Wrapped native, D&& del)
    //     : _native(native)
    //     , _del(new NativeDeleterImpl<value_type, D> {
    //         std::forward<D>(del)
    //     }) 
    // {  };

    template <typename D>
    NativeWrapper(D del) requires(pol == StorePolitics::Direct)
        : _native()
        , _del(new NativeDeleterImpl<value_type, D> {
            std::forward<D>(del)
        }) 
    {  };

    template <typename D>
    NativeWrapper(D&& del) requires(pol == StorePolitics::Shared)
        : _native(new Native{}, del)
    {  };

    NativeWrapper(const Internal& oth)
        : _native(oth._native)
        , _del(oth._del) 
    {  };


    NativeWrapper()
        : _native()
        , _del() 
    {  };

    ~NativeWrapper() {
        _del.release(native());
    }

    const value_type& native() const noexcept {
        return get(_native);
    }

    operator value_type() const noexcept {
        return native();
    }

    value_type& native() noexcept {
        return get(_native);
    }

    operator value_type() noexcept {
        return native();
    }

protected:
    void native(const value_type& oth) noexcept {
        set(_native, oth);
    }

private:
    template < typename T > const T&
    get(std::shared_ptr<T> value) const noexcept {
        return *value;
    }

    template < typename T > const T&
    get(const T& value) const noexcept {
        return value;
    }

    template < typename T > T&
    get(std::shared_ptr<T> value) noexcept {
        return *value;
    }

    template < typename T > T&
    get(T& value) noexcept {
        return value;
    }

    template < typename T > void
    set(std::shared_ptr<T> native, const T& value) noexcept {
        native.swap(std::make_shared<T>(value));
    }

    template < typename T > void
    set(T& native, const T& value) noexcept {
        native = value;
    }
}; // NativeWrapper
}; // vk