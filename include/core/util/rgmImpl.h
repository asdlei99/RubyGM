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

#include "../Util/rgmStruct.h"
#include <d2d1_3.h>

// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // get time
    auto get_time() noexcept->uint32_t;
    // d2d
    inline auto&d2d(const RubyGM::Point2F& pt) { return reinterpret_cast<const D2D1_POINT_2F&>(pt); }
    // rubygm
    inline auto&rubygm(const D2D1_POINT_2F& pt) { return reinterpret_cast<const RubyGM::Point2F&>(pt); }
    // d2d
    inline auto&d2d(const RubyGM::ColorF& c) { return reinterpret_cast<const D2D1_COLOR_F&>(c); }
    // rubygm
    inline auto&rubygm(const D2D1_COLOR_F& c) { return reinterpret_cast<const RubyGM::ColorF&>(c); }
    // d2d
    inline auto&d2d(RubyGM::Matrix3X2F& mt) { return reinterpret_cast<D2D1_MATRIX_3X2_F&>(mt); }
    // rubygm
    inline auto&rubygm(D2D1_MATRIX_3X2_F& mt) { return reinterpret_cast<RubyGM::Matrix3X2F&>(mt); }
    // d2d
    inline auto&d2d(const RubyGM::Matrix3X2F& mt) { return reinterpret_cast<const D2D1_MATRIX_3X2_F&>(mt); }
    // rubygm
    inline auto&rubygm(const D2D1_MATRIX_3X2_F& mt) { return reinterpret_cast<const RubyGM::Matrix3X2F&>(mt); }
    // d2d
    inline auto&d2d(const RubyGM::RectF& mt) { return reinterpret_cast<const D2D1_RECT_F&>(mt); }
    // rubygm
    inline auto&rubygm(const D2D1_RECT_F& mt) { return reinterpret_cast<const RubyGM::RectF&>(mt); }
    // ---------------------------
    template<class T, class Y, class U>
    auto d2d(Y* obj, const U& iid) noexcept -> T* {
#ifdef _DEBUG
        if(obj) {
            void* brush = nullptr;
            assert(SUCCEEDED(obj->QueryInterface(iid, &brush)));
            if (brush) ((IUnknown*)(brush))->Release();
        }
#endif
        auto super = static_cast<typename Y::Super*>(obj);
        return static_cast<T*>(super);
    }
}}
