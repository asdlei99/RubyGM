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

// rubygm namespace
namespace RubyGM { 
    // error code type
    using Result = int32_t;
    // small alloc, alloc for small size, cannot return nullptr, nor std::terminate
    void*SmallAlloc(size_t len /* < 256*/) noexcept;
    // small alloc
    template<size_t len> inline auto SmallAlloc() noexcept {
        static_assert(len < 256, "too large");
        return RubyGM::SmallAlloc(len);
    }
    // small alloc
    template<typename T> inline auto SmallAlloc() noexcept {
        return RubyGM::SmallAlloc<sizeof(T)>();
    }
    // small free
    void SmallFree(void* ptr) noexcept;
    // alloc for drawable object
    auto NormalAlloc(size_t len) noexcept->void*;
    // free for drawable object
    void NormalFree(void* ptr) noexcept;
    // aligned len of size_t
    constexpr size_t AlignedSizeTLen(size_t l) noexcept {
        return (l + sizeof(size_t) - size_t(1)) / sizeof(size_t);
    }
    // release in safe way
    template<typename T> inline auto SafeRelease(T*& obj) noexcept {
        if (obj) obj->Release(); obj = nullptr;
    }
    // call addref in safe way
    template<typename T> inline auto SafeAcquire(T* obj) {
        if (obj) obj->AddRef();
        return obj;
    }
}

