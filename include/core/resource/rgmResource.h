﻿#pragma once
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

#ifndef RUBYGM_NOVTABLE
#ifdef _MSC_VER
#define RUBYGM_NOVTABLE __declspec(novtable)
#else
#define RUBYGM_NOVTABLE
#endif
#endif

// rubygm namespace
namespace RubyGM {
    // bitmap
    struct IGMBitmap;
    // brush
    struct IGMBrush;
    // resource namespace
    namespace Resource {
        // helper for LowOccupancy
        template<class T> 
        inline void LowOccupancyHelper(T*& ptr) noexcept {
            if (ptr) {
                if (ptr->Release())  ptr->AddRef();
                else ptr = nullptr;
            }
        }
        // base class
        struct RUBYGM_NOVTABLE Base {
            // dispose
            virtual void Dispose() noexcept = 0;
            // recreate
            virtual auto Recreate() noexcept ->uint32_t = 0;
            // set to Low Occupancy to save memory
            virtual void LowOccupancy() noexcept = 0;
        };
        // bitmap resource
        struct RUBYGM_NOVTABLE Bitmap : Base {
            // get bitmap
            virtual auto GetBitmap() noexcept -> IGMBitmap* = 0;
        };
        // brush resource
        struct RUBYGM_NOVTABLE Brush : Base {
            // get bitmap
            virtual auto GetBrush() noexcept -> IGMBrush* = 0;
        };
    }
}