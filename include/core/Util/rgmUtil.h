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
        template<uint32_t index>  auto Test() const noexcept { 
            static_assert(index<LENGTH,"bad"); 
            return !!(m_data & (1 << index));
        }
        // set to true
        template<uint32_t index> auto SetTrue() noexcept { 
            static_assert(index<LENGTH,"bad"); 
            m_data |= (1 << index); 
        };
        // set to false
        template<uint32_t index> auto SetFalse() noexcept { 
            static_assert(index<LENGTH,"bad"); 
            m_data &= ~(1 << index);
        };
        // set to NOT
        template<uint32_t index> auto SetNot() noexcept { 
            static_assert(index<LENGTH,"bad"); 
           m_data ^= (1 << index); 
        };
        // set to???
        template<uint32_t index, typename V> auto SetTo(V value) noexcept { 
            static_assert(index<LENGTH,"bad"); 
            this->SetFalse<index>(); 
            m_data |= (!!(value) << index);
        }
    private:
        // data for bit-array
        T           m_data = T(0);
    };
    // refcounted smart pointer
    template<class T> class RefPtr {
        // friend
        template<typename U> friend class RefPtr;
    public:
        // dtor
        ~RefPtr() noexcept { this->safe_release(); }
        // dispose
        void Dispose() noexcept { this->safe_release(); }
        // ctor
        RefPtr(T*& ptr) noexcept : m_pObject(ptr) { this->safe_addref(); }
        // ctor with move ptr
        RefPtr(T*&& ptr) noexcept : m_pObject(ptr) { ptr = nullptr; }
        // ctor with RefPtr
        template<class U>
        RefPtr(const RefPtr<U>& obj) noexcept : m_pObject(obj.m_pObject) { this->safe_addref(); }
        // ctor with move RefPtr
        template<class U>
        RefPtr(RefPtr<U>&& obj) noexcept : m_pObject(obj.m_pObject) { obj.m_pObject = nullptr; }
        // ctor with RefPtr
        RefPtr(const RefPtr<T>& obj) noexcept : m_pObject(obj.m_pObject) { this->safe_addref(); }
        // ctor with move RefPtr
        RefPtr(RefPtr<T>&& obj) noexcept : m_pObject(obj.m_pObject) { obj.m_pObject = nullptr; }
        // pointer-like
        auto operator->() const noexcept { return m_pObject; }
        // pointer-like
        auto&operator*() const noexcept { return *m_pObject; }
        // ptr
        auto Ptr() const noexcept -> T* { return m_pObject; }
        // operator =
        template<class U> auto& operator=(const RefPtr<U>& obj) noexcept { 
            assert(this != &obj);
            this->safe_release(); m_pObject = obj.m_pObject; this->safe_addref();
            return *this; 
        }
        // operator =
        template<class U> auto& operator=(RefPtr<U>&& obj) noexcept { 
            assert(this != &obj);
            this->safe_release(); m_pObject = obj.m_pObject; 
            obj.m_pObject = nullptr;
            return *this; 
        }
        // operator =
        auto& operator=(const RefPtr<T>& obj) noexcept { 
            assert(this != &obj);
            this->safe_release(); m_pObject = obj.m_pObject; this->safe_addref();
            return *this; 
        }
        // operator =
        auto& operator=(RefPtr<T>&& obj) noexcept { 
            assert(this != &obj);
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
}