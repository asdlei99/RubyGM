#pragma once
/**
* Copyright (c) 2015-2016 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include <cstdint>
#include <cassert>
#include <new>
#include "../rubygm.h"

// rubygm namespace
namespace RubyGM {
    // small alloc, alloc for small size, cannot return nullptr, nor std::terminate
    auto SmallAlloc(size_t len /* < 256*/) noexcept->void*;
    // small free
    void SmallFree(void* ptr) noexcept;
    // Bit Array
    template<typename T> class CGMBitArray {
    public:
        // bit wide
        enum : size_t { LENGTH = sizeof(T) * CHAR_BIT };
        // ctor
        CGMBitArray() noexcept {};
        // ctor
        CGMBitArray(T v) noexcept : m_data(v) {};
        // dtor
        ~CGMBitArray() noexcept {};
        // is true or fasle
        auto Test(uint32_t index) const noexcept { assert(index<LENGTH); return !!(m_data & (1 << index)); }
        // set to true
        auto SetTrue(uint32_t index) noexcept { assert(index<LENGTH); m_data |= (1 << index); };
        // set to false
        auto SetFalse(uint32_t index) noexcept { assert(index<LENGTH); m_data &= ~(1 << index); };
        // set to NOT
        auto SetNot(uint32_t index) noexcept { assert(index<LENGTH); m_data ^= (1 << index); };
        // set to???
        template<typename V>
        auto SetTo(uint32_t index, V value) noexcept { assert(index<LENGTH); this->SetFalse(index); m_data |= (!!(value) << index); }
    private:
        // data for bit-array
        T           m_data = T(0);
    };
    // singleton
    template<typename T>
    struct CGMSingleton {
        // ptr
        auto Ptr() noexcept { return reinterpret_cast<T*>(this->buffer); }
        // create
        template<typename ...Args> auto Create(Args... args) {
#ifdef _DEBUG
            assert(state == 0 && "create this after destory!");
            state++;
#endif
            return new(this->buffer) T(args...);
        }
        // destory
        auto Destory() noexcept {
#ifdef _DEBUG
            assert(state != 0 && "destory this after create!");
            --state;
#endif
            this->Ptr()->~T();
        }
#ifdef _DEBUG
        // state
        size_t              state = 0;
#endif
        // buffer for  singleton
        alignas(T)  char    buffer[sizeof(T)];
        // singleton
        static CGMSingleton<T>      s_instance;
    };
    // impl namespace
    namespace impl {
        // sfinae
        template<class> struct sfinae_true : std::true_type{};
        // test if has AddRef member funtion
        template<class T>
        static auto test_addref(int) -> sfinae_true<decltype(std::declval<T>().AddRef())>;
        // test if has AddRef member funtion
        template<class>
        static auto test_addref(long) -> std::false_type;
        // has addref
        template<class T>
        struct has_addref : decltype(impl::test_addref<T>(0)){};
    }
    // refcounted smart pointer
    template<class T> class CGMPtrA {
        // friend
        template<typename U> friend class CGMPtrA;
    public:
        // dtor
        ~CGMPtrA() noexcept { this->safe_release(); }
        // dispose
        void Dispose() noexcept { this->safe_release(); }
        // ctor
        CGMPtrA(const T*& ptr) noexcept : m_pObject(ptr) { this->safe_addref(); }
        // ctor with move ptr
        CGMPtrA(T*&& ptr) noexcept : m_pObject(ptr) { ptr = nullptr; }
        // ctor with CGMPtrA
        CGMPtrA(const CGMPtrA<T>& obj) noexcept : m_pObject(obj.m_pObject) { this->safe_addref(); }
        // ctor with move CGMPtrA
        CGMPtrA(CGMPtrA<T>&& obj) noexcept : m_pObject(obj.m_pObject) { obj.m_pObject = nullptr; }
        // pointer-like
        auto operator->() noexcept { return m_pObject; }
        // ptr
        auto Ptr() const noexcept -> T* { return m_pObject; }
        // operator =
        template<class U> auto& operator=(const CGMPtrA<U>& obj) noexcept { 
            this->safe_release(); m_pObject = obj.m_pObject; this->safe_addref();
            return *this; 
        }
        // operator =
        template<class U> auto& operator=(CGMPtrA<U>&& obj) noexcept { 
            this->safe_release(); m_pObject = obj.m_pObject; 
            obj.m_pObject = nullptr;
            return *this; 
        }
        // bool
        operator bool() const noexcept { return m_pObject != nullptr; }
    private:
        // zero
        void zero() noexcept { m_pObject = nullptr; }
        // safe add ref
        void safe_addref() noexcept { RubyGM::SafeAcquire(m_pObject); }
        // safe release
        void safe_release() noexcept { RubyGM::SafeRelease(m_pObject); }
    private:
        // pointer to object
        T*              m_pObject;
    };
    // dispose smart pointer
    template<class T> class CGMPtrB {
        // friend
        template<typename U> friend class CGMPtrB;
    public:
        // dtor
        ~CGMPtrB() noexcept { this->safe_dispose(); }
        // dispose
        void Dispose() noexcept { this->safe_dispose(); }
        // ctor
        CGMPtrB(const T*& ptr) noexcept = delete;
        // ctor with move ptr
        CGMPtrB(T*&& ptr) noexcept : m_pObject(ptr) { ptr = nullptr; }
        // ctor with CGMPtrB
        CGMPtrB(const CGMPtrB<T>& obj) = delete;
        // ctor with move CGMPtrB
        CGMPtrB(CGMPtrB<T>&& obj) noexcept : m_pObject(obj.m_pObject) { obj.m_pObject = nullptr; }
        // pointer-like
        auto operator->() noexcept { return m_pObject; }
        // ptr
        auto Ptr() const noexcept -> T* { return m_pObject; }
        // operator =
        template<class U> auto& operator=(const CGMPtrB<U>& obj) noexcept = delete;
        // operator =
        template<class U> auto& operator=(CGMPtrB<U>&& obj) noexcept { 
            this->safe_dispose(); m_pObject = obj.m_pObject; obj.m_pObject = nullptr;
            return *this; 
        }
        // bool
        operator bool() const noexcept { return m_pObject != nullptr; }
    private:
        // zero
        void zero() noexcept { m_pObject = nullptr; }
        // safe dispose
        void safe_dispose() noexcept { if (m_pObject) m_pObject->Dispose(); m_pObject = nullptr; }
    private:
        // pointer to object
        T*              m_pObject;
    };
    // impl
    namespace impl {
        // sp helper
        template<class T, bool U> struct sp_helper;
        // has AddRef
        template<class T> struct sp_helper<T, true> { using Class = CGMPtrA<T>; };
        // no AddRef
        template<class T> struct sp_helper<T, false> { using Class = CGMPtrB<T>; };
    }
    // normal pointer
    template<typename T>
    using CGMPtr = typename impl::sp_helper<T, impl::has_addref<T>::value>::Class;
}