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

#include "../rgmDrawable.h"
#include "../../Util/rgmUtil.h"
#include "../../text/rgmTextStruct.h"
#include <utility>

// rubygm namespace
namespace RubyGM { 
    // text font
    struct IGMFont;
    // text layout
    struct IGMTextlayout;
    // Drawable namespace
    namespace Drawable {
        // text layout 
        class Textlayout : public Base {
            // super class
            using Super = Base;
        public:
            // create this
            static auto Create() noexcept->Textlayout*;
            // create this
            static auto CreateSP() noexcept {
                return std::move(RubyGM::CGMPtr<Drawable::Textlayout>(
                    std::move(Textlayout::Create()))
                );
            }
        private:
            // ctor
            Textlayout() noexcept;
            // ctor
            Textlayout(const Textlayout&) = delete;
            // ctor
            ~Textlayout() noexcept;
        public:
            // render object
            void Render(IGMRednerContext& rc) const noexcept override;
            // recreate
            auto Recreate() noexcept->uint32_t;
        private:
            // dispose object
            void dispose() noexcept override;
        public:
            // get text legnth
            auto GetTextLength() const noexcept { return m_uTextLength; }
            // set layout size
            void SetLayoutSize(float w, float h) noexcept;
            // get text metrics
            void GetMetrics(TextMetrics&) const noexcept;
            // set text font size
            void SetFontSize(TextRange range, float size) noexcept;
            // set text font color
            void SetFontColor(TextRange range, const ColorF& color) noexcept;
            // set text underline
            void SetUnderline(TextRange range, bool underline) noexcept;
            // set strikethrough
            void SetStrikethrough(TextRange range, bool underline) noexcept;
        protected:
            // text layout
            IGMTextlayout*          m_pTextlayout = nullptr;
            // text layout width
            float                   m_fWidth = 128.f;
            // text layout hright
            float                   m_fHeight = 32.f;
            // text length
            uint32_t                m_uTextLength = 0;
            // unused
            uint32_t                m_unused = 0;
        };
    }
}