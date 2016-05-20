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

#include "rgmVector.h"
#include "../Util/rgmUtil.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // graphics interface for geometry
    struct IGMGeometry;
    // Drawable namespace
    namespace Drawable {
        // pointer
        class Textlayout;
        // status for Glyph
        struct GlyphStatus : VectorStatus {
            //
            Textlayout*     layout;
            // ctor
            GlyphStatus() : VectorStatus() {}
            // default value
            inline GlyphStatus(Default v) : VectorStatus(v) {
                layout = nullptr;
            }
        };
        // line 
        class Glyph : public Drawable::Vector {
            // super class
            using Super = Drawable::Vector;
            // dispose object
            void dispose() noexcept override;
        protected:
            // ctor
            Glyph(const GlyphStatus&) noexcept;
            // ctor
            ~Glyph() noexcept;
        public:
            // create this
            static auto Create(const GlyphStatus&) noexcept ->Glyph*;
            // create this
            static auto CreateSP(const GlyphStatus& ls) noexcept {
                return std::move(RubyGM::CGMPtrA<Drawable::Glyph>(
                    std::move(Glyph::Create(ls)))
                );
            }
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        protected:
            // recreate
            //auto recreate() noexcept -> Result override;
        public:
            // 
            IGMGeometry*        m_pTextGlyph = nullptr;
        };
    }
}