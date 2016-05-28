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
#include "../../game/rgmResource.h"

// rubygm namespace
namespace RubyGM {
    // matrix
    struct Matrix3X2F;
    // bitmap
    struct IGMBitmap;
    // brush
    struct IGMBrush;
    // stroke style
    struct IGMStrokeStyle;
    // asset namespace
    namespace Asset {
        // asset object
        struct RUBYGM_NOVTABLE Object : RubyGM::Base::Resource {
            // set to Low Occupancy to save memory
            virtual void LowOccupancy() noexcept = 0;
            // helper for LowOccupancy
            template<class T> 
            inline void LowOccupancyHelper(T*& ptr) noexcept {
                if (!ptr) return;
                if (ptr->Release()) ptr->AddRef();
                else ptr = nullptr;
            }
        protected:
            // recreate... 
            virtual auto recreate() noexcept ->Result { return 0; };
        };
        // brush resource
        struct RUBYGM_NOVTABLE Brush : RubyGM::Asset::Object {
            // get brush
            virtual auto GetBrush() noexcept -> IGMBrush* = 0;
            // set opacity
            virtual void SetOpacity(float opacity) noexcept = 0;
            // set transfrom
            virtual void SetTransfrom(const Matrix3X2F&) noexcept = 0;
            // get opacity
            virtual auto GetOpacity() const noexcept -> float = 0;
            // get transfrom
            virtual void GetTransfrom(Matrix3X2F&) const noexcept = 0;
        };
        // bitmap resource
        struct RUBYGM_NOVTABLE Bitmap : RubyGM::Asset::Object {
            // get bitmap
            virtual auto GetBitmap() noexcept -> IGMBitmap* = 0;
            // redraw the content if self is RasterBitmap
            virtual auto Redraw() noexcept ->Result { return Result(0); };
            // save as png file
            auto SaveAsPng(const wchar_t* file_name) noexcept ->Result;
            // save as png file, width utf-8 string
            auto SaveAsPng(const char* file_name) noexcept ->Result;
        };
        // raster bitmap
        struct RUBYGM_NOVTABLE RasterBitmap : Asset::Bitmap {

        };
    }
}
