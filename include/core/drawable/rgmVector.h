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

#include "rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include "../asset/rgmAsset.h"
#include "../asset/rgmAssetStroke.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // Drawable namespace
    namespace Drawable {
        // status for basic vector graphics
        struct VectorStatus : BaseStatus {
            // stroke color, valid on stroke brush invalid
            // set alpha to 0.0f to undisplay this part
            ColorF          stroke_color;
            // _fill_ color, valid on _fill_ brush invalid
            // set alpha to 0.0f to undisplay this part
            ColorF          fill_color;
            // _fill_ brush, set null to use none-style stroke
            Asset::Stroke*  stroke_style;
            // stroke brush, set null to use stroke color
            Asset::Brush*   stroke_brush;
            // _fill_ brush, set null to use _fill_ color
            Asset::Brush*   fill_brush;
            // stroke width
            float           stroke_width;
            // ctor
            VectorStatus() : BaseStatus() {}
            // ctor
            VectorStatus(Default v) : BaseStatus(v) {
                stroke_color.r = stroke_color.g = stroke_color.b = 0.f;
                stroke_color.a = 1.f;   fill_color = stroke_color;
                stroke_style = nullptr; stroke_brush = nullptr; 
                fill_brush = nullptr;   stroke_width = 1.f;
            }
        };
        // drawable vector graphics
        class Vector : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
            // reset asset
            auto reset_asset() noexcept -> Result;
        protected:
            // ctor
            Vector(const VectorStatus&) noexcept;
            // ctor
            ~Vector() noexcept;
        protected:
            // recreate
            auto recreate() noexcept -> Result override;
            // set stroke brush
            void set_stroke_brush() const noexcept;
            // set fill brush
            void set_fill_brush() const noexcept;
            // is draw stroke
            bool is_draw_stroke() const { return stroke_color.a != 0.f; }
            // is draw fill
            bool is_draw_fill() const { return fill_color.a != 0.f; }
        public:
            // stroke color, 0.0 alpha will undisplay this part
            ColorF                  stroke_color{0.f};
            // fill color, 0.0 alpha will undisplay this part
            ColorF                  fill_color{0.f};
        protected:
            // stroke style asset
            Asset::Stroke*          m_pAsStrokeStyle = nullptr;
            // stroke brush asset
            Asset::Brush*           m_pAsBrushStroke = nullptr;
            // fill brush asset
            Asset::Brush*           m_pAsBrushFill = nullptr;
            // stroke style GI data(graphics-interface)
            IGMStrokeStyle*         m_pGiStrokeStyle = nullptr;
            // stroke brush GI data
            IGMBrush*               m_pGiBrushStroke = nullptr;
            // fill brush GI data
            IGMBrush*               m_pGiBrushFill = nullptr;
            // is using stroke color
            bool                    m_bUseStrokeColor = false;
            // is using fill color
            bool                    m_bUseFillColor = false;
            // unused bool data
            bool                    m_unused_bool[2];
        public:
            // stroke width
            float                   stroke_width = 1.f;
        };
    }
}