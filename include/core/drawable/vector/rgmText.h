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

#include "rgmGeometry.h"
#include "../rgmTextlayout.h"
#include "../../Util/rgmUtil.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // graphics interface for geometry
    struct IGMGeometry;
    // Drawable namespace
    namespace Drawable {
        // status for Text
        struct TextStatus : GeometryStatus {
            // layout
            RefPtr<Textlayout>     layout;
            // default  - copy
            inline TextStatus(RefPtr<Textlayout>& l) : 
                layout(l), GeometryStatus() {
            }
            // default  - move
            inline TextStatus(RefPtr<Textlayout>&&l) : 
                layout(std::move(l)), GeometryStatus() {
            }
        };
        // text geometry
        class Text : public Drawable::Geometry {
            // super class
            using Super = Drawable::Geometry;
            // dispose object
            void dispose() noexcept override;
        protected:
            // ctor
            Text(const TextStatus&) noexcept;
            // ctor
            ~Text() noexcept;
        public:
            // create this
            static auto Create(const TextStatus&) noexcept ->Text*;
            // create this
            static auto CreateSP(const TextStatus& ls) noexcept {
                return RefPtr<Drawable::Text>(Text::Create(ls));
            }
            // render object
            //void Render(IGMRenderContext& rc) const noexcept override;
        };
    }
}