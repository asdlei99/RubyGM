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
#include "../rubygm.h"

// rubygm namespace
namespace RubyGM {
    // cache namespace
    namespace Cache {
        // remove all cahce files
        auto RemoveAllFiles() noexcept->Result;
        // get directory, return null for buffer too small
        auto GetDirName(wchar_t path[], size_t len) noexcept->wchar_t*;
        // get directory string length with null-end-char
        auto GetDirNameLength() noexcept->uint32_t;
        // write file
        auto WriteData(size_t id, void* data, size_t len) noexcept->Result;
        // write file
        auto ReadData(size_t id, void* data, size_t len) noexcept->Result;
        // write file
        inline auto WriteData(void* id, void* data, size_t len) noexcept {
            return Cache::WriteData(reinterpret_cast<size_t>(id), data, len);
        }
        // write file
        inline auto ReadData(void* id, void* data, size_t len) noexcept {
            return Cache::ReadData(reinterpret_cast<size_t>(id), data, len);
        }
    }
}

