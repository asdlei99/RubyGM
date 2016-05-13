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
#include "../text/rgmTextStruct.h"
#include "../asset/rgmAssetFont.h"
#include <utility>

// rubygm namespace
namespace RubyGM { 
    // text font
    struct IGMFont;
    // text layout
    struct IGMTextlayout;
    // text renderer
    struct IGMTextRenderer;
    // Drawable namespace
    namespace Drawable {
        // status for textlauout
        struct TextStatus : BaseStatus {
            // text renderer
            const char*             renderer;
            // text context
            const char*             context;
            // text
            const wchar_t*          text;
            // font resource
            Asset::Font&            font;
            // layout width
            float                   width;
            // layout height
            float                   height;
            // text length
            uint32_t                textlen;
            // ctor
            ~TextStatus() noexcept { font.Release(); }
            // default ctor
            inline TextStatus(Asset::Font&& f) : font(f) { }
            // default value
            inline TextStatus(Asset::Font&& f, Default v) : 
                BaseStatus(v), font(f) {
                renderer = ""; context = "";
                text = L""; textlen = 0;
                width = 128.f; height = 32.f;
            }
        };
        // text layout 
        class Textlayout : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
        public:
            // create this
            static auto Create(const TextStatus&) noexcept->Textlayout*;
            // create this
            static auto CreateSP(const TextStatus& ts) noexcept {
                return std::move(RubyGM::CGMPtr<RubyGM::Drawable::Textlayout>(
                    std::move(Textlayout::Create(ts)))
                );
            }
        private:
            // ctor
            Textlayout(const TextStatus&) noexcept;
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
            // set layout width
            auto SetLayoutWidth(float width) noexcept ->uint32_t;
            // set layout height
            auto SetLayoutHeight(float h) noexcept ->uint32_t;
            // get text metrics
            auto GetMetrics(TextMetrics&) const noexcept ->uint32_t;
            // get line count
            auto GetLineCount() const noexcept ->uint32_t;
            // get line count
            auto GetLineMetrics(uint32_t buflen, LineMetrics* buf) const noexcept ->uint32_t;
            // set valign
            auto SetVAlignment(VAlignment va) noexcept->uint32_t;
            // set halign
            auto SetHAlignment(HAlignment ha) noexcept->uint32_t;
            // set reading direction
            auto SetReadingDirection(ReadDirection reading) noexcept->uint32_t;
            // set flow direction
            auto SetFlowDirection(FlowDirection flow) noexcept->uint32_t;
            // set word
            auto SetWordWrapping(WordWrapping warp) noexcept->uint32_t;
            // set text font weight
            auto SetFontWeight(TextRange range, FontWeight fw) noexcept ->uint32_t;
            // set text font style
            auto SetFontStyle(TextRange range, FontStyle fs) noexcept ->uint32_t;
            // set text font stretch
            auto SetFontStretch(TextRange range, FontStretch fs) noexcept ->uint32_t;
            // set text font size
            auto SetFontSize(TextRange range, float size) noexcept ->uint32_t;
            // set text font name
            auto SetFontName(TextRange range, const wchar_t*) noexcept ->uint32_t;
            // set text font color
            auto SetFontColor(TextRange range, const ColorF& color) noexcept ->uint32_t;
            // set text underline
            auto SetUnderline(TextRange range, bool underline) noexcept ->uint32_t;
            // set strikethrough
            auto SetStrikethrough(TextRange, bool strike) noexcept ->uint32_t;
            // hit test from point, output bool : is trailing, is inside
            auto HittestPoint(Point2F, bool[2], HittestMetrics&) noexcept ->uint32_t;
            // hit test from text pos
            auto HittesTextPos(uint32_t, bool trailing, Point2F&, HittestMetrics&) noexcept ->uint32_t;
            // hit test from text pos
            auto HittesTextRange(TextRange, uint32_t buflen, HittestMetrics* buf) noexcept ->uint32_t;
            // hit test from text pos
            auto HittesTextRangeGetCount(TextRange range) const noexcept ->uint32_t;
        protected:
            // text renderer
            IGMTextRenderer*        m_pTextRenderer = nullptr;
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
            // draw context
            alignas(size_t) size_t  m_bufDrawContext[0];
        };
    }
}