#pragma once
// this header included all drawable class and overload a
// friendly funtion :   RubyGM::Drawable::CreateSP



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

// basic graphics
#include "rgmBitmap.h"
#include "rgmTextlayout.h"
// vector graphics
#include "vector/rgmLine.h"
#include "vector/rgmRect.h"
#include "vector/rgmText.h"
#include "vector/rgmPath.h"
#include "vector/rgmPolygon.h"
#include "vector/rgmEllipse.h"
#include "vector/rgmPolyline.h"

// effect image
#include "effect/rgmBlur.h"
#include "effect/rgmShadow.h"

// rubygm namespace
namespace RubyGM { 
    // Drawable namespace
    namespace Drawable {
        // ====================================================================
        // ============================= Basic ================================
        // ====================================================================
        // create bitmap
        inline auto CreateSP(const BitmapStatus& bs) noexcept {
            return std::move(Bitmap::CreateSP(bs));
        }
        // create text layout
        inline auto CreateSP(const TextlayoutStatus& tls) noexcept {
            return std::move(Textlayout::CreateSP(tls));
        }
        // ====================================================================
        // ========================= Vector Graphics ==========================
        // ====================================================================
        // create line vector graphics
        inline auto CreateSP(const LineStatus& ls) noexcept {
            return std::move(Line::CreateSP(ls));
        }
        // create rect vector graphics
        inline auto CreateSP(const RectStatus& rs) noexcept {
            return std::move(Rect::CreateSP(rs));
        }
        // create text vector graphics
        inline auto CreateSP(const TextStatus& ts) noexcept {
            return std::move(Text::CreateSP(ts));
        }
        // create path vector graphics
        inline auto CreateSP(const PathStatus& ps) noexcept {
            return std::move(Path::CreateSP(ps));
        }
        // create polygon vector graphics
        inline auto CreateSP(const PolygonStatus& ps) noexcept {
            return std::move(Polygon::CreateSP(ps));
        }
        // create ellipse vector graphics
        inline auto CreateSP(const EllipseStatus& es) noexcept {
            return std::move(Ellipse::CreateSP(es));
        }
        // create text polyline graphics
        inline auto CreateSP(const PolylineStatus& ps) noexcept {
            return std::move(Polyline::CreateSP(ps));
        }
        // ====================================================================
        // ========================== Effect Image ============================
        // ====================================================================
        // create gaussian blur effect
        inline auto CreateSP(const GaussianBlurStatus& gbs) noexcept {
            return std::move(GaussianBlur::CreateSP(gbs));
        }
        // create shadow effect
        inline auto CreateSP(const ShadowStatus& ss) noexcept {
            return std::move(Shadow::CreateSP(ss));
        }
    }
}