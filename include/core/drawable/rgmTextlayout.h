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
        // status for textlayout
        struct TextlayoutStatus : BaseStatus {
            // basic color
            ColorF                  color;
            // font, use default font if set to null
            RefPtr<Asset::Font>     font;
            // text renderer name, c-style string required
            // could be: "normal", "outline"...
            const char*             renderer;
            // text context, c-style string required
            // for "normal" renderer: none
            // for "outline"renderer: "stroke-width[, stroke-color]"
            const char*             context;
            // text, c-style string NOT required
            const wchar_t*          text;
            // layout width
            float                   width;
            // layout height
            float                   height;
            // text length
            uint32_t                textlen;
            // xml rich-text-format?  
            // like:  This is<font color="#6CF"> Maya Blue </font>
            // if set but invalid xml given it will ignore this flag
            uint32_t                isxml;
            // default value
            inline TextlayoutStatus() : font(nullptr), BaseStatus() {
                renderer = ""; context = "";
                text = L""; textlen = 0;
                width = 128.f; height = 32.f;
                color.r = color.g = color.b = 0.f; color.a = 1.f;
                isxml = false;
            }
        };
        // editable text layout 
        //struct EditableStatus : TextlayoutStatus { };
        // text layout 
        class Textlayout : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
            // frirend
            friend class Text;
            // dispose object
            void dispose() noexcept override;
            // get context 
            inline auto get_context() const noexcept { 
                auto end = reinterpret_cast<const void*>(this + 1);
                return const_cast<void*>(end); 
            }
        public:
            // create this
            static auto Create(const TextlayoutStatus&) noexcept->Textlayout*;
            // create this
            static auto CreateSP(const TextlayoutStatus& ts) noexcept {
                return RefPtr<Drawable::Textlayout>(Textlayout::Create(ts));
            }
        protected:
            // ctor
            Textlayout(const TextlayoutStatus&) noexcept;
            // ctor
            Textlayout(const Textlayout&) = delete;
            // ctor
            ~Textlayout() noexcept;
            // recreate
            auto recreate() noexcept ->Result override final;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override ;
            // Rasterization
            using Super::Rasterization;
            // rasterization helper
            auto Rasterization(float sf) noexcept->Drawable::Bitmap* {
                return this->Rasterization(sf, this->GetSizeFromMetrics());
            }
            // rasterization helper
            auto Rasterization(SizeF sf) noexcept->Drawable::Bitmap* {
                return this->Rasterization(sf, this->GetSizeFromMetrics());
            }
#if 0
            // rasterization helper
            auto RasterizationSP(SizeF sf) noexcept {
                return RefPtr<Drawable::Bitmap>(this->Rasterization(sf));
            }
            // rasterization helper
            auto RasterizationSP(float sf) noexcept {
                return RefPtr<Drawable::Bitmap>(this->Rasterization(sf));
            }
#else
            // rasterization helper
            template<typename ...Args>
            auto RasterizationSP(Args&&... args) noexcept {
                return RefPtr<Drawable::Bitmap>(
                    this->Rasterization(std::forward<Args>(args)...)
                    );
            }
#endif
        public:
            // get text legnth
            auto GetTextLength() const noexcept { return m_uTextLength; }
            // get size from metrics
            auto GetSizeFromMetrics() const noexcept->SizeF;
            // set layout width
            auto SetLayoutWidth(float width) noexcept ->Result;
            // set layout height
            auto SetLayoutHeight(float h) noexcept ->Result;
            // get text metrics
            auto GetMetrics(TextMetrics&) const noexcept ->Result;
            // get line count
            auto GetLineCount() const noexcept ->uint32_t;
            // get line count
            auto GetLineMetrics(uint32_t buflen, LineMetrics* buf) const noexcept ->Result;
            // set valign
            auto SetVAlignment(VAlignment va) noexcept->Result;
            // set halign
            auto SetHAlignment(HAlignment ha) noexcept->Result;
            // set reading direction
            auto SetReadingDirection(ReadDirection reading) noexcept->Result;
            // set flow direction
            auto SetFlowDirection(FlowDirection flow) noexcept->Result;
            // set word
            auto SetWordWrapping(WordWrapping warp) noexcept->Result;
            // set text font weight
            auto SetFontWeight(TextRange range, FontWeight fw) noexcept ->Result;
            // set text font style
            auto SetFontStyle(TextRange range, FontStyle fs) noexcept ->Result;
            // set text font stretch
            auto SetFontStretch(TextRange range, FontStretch fs) noexcept ->Result;
            // set text font size
            auto SetFontSize(TextRange range, float size) noexcept ->Result;
            // set text font name
            auto SetFontName(TextRange range, const wchar_t*) noexcept ->Result;
            // set text font color
            auto SetFontColor(TextRange range, const ColorF& color) noexcept ->Result;
            // set text underline
            auto SetUnderline(TextRange range, bool underline) noexcept ->Result;
            // set strikethrough
            auto SetStrikethrough(TextRange, bool strike) noexcept ->Result;
            // set font feature(like "smcp" "smcp ruby")
            auto SetFontFeature(TextRange range, const char* feature) noexcept ->Result;
            // hit test from point, output bool : is trailing, is inside
            auto HittestPoint(Point2F, bool[2], HittestMetrics&) noexcept ->Result;
            // hit test from text pos
            auto HittesTextPos(uint32_t, bool trailing, Point2F&, HittestMetrics&) noexcept ->Result;
            // hit test from text pos
            auto HittesTextRange(TextRange, uint32_t buflen, HittestMetrics* buf) noexcept ->Result;
            // hit test from text pos
            auto HittesTextRangeGetCount(TextRange range) const noexcept ->Result;
        public:
            // basic color
            ColorF                  basic_color;
        protected:
            // text renderer
            IGMTextRenderer*        m_pTextRenderer = nullptr;
            // text layout
            IGMTextlayout*          m_pTextlayout = nullptr;
            // text layout width
            float                   m_fWidth = 128.f;
            // text layout height
            float                   m_fHeight = 32.f;
            // text length
            uint32_t                m_uTextLength = 0;
            // unused u32
            uint32_t                m_unused_u32 = 0;
            // buffer
            size_t                  m_buffer[0];
        };
       /* // editable text layout
        class Editable final : public Textlayout {
            // get context 
            auto get_context() const noexcept;
        public:
            // create this
            static auto Create(const EditableStatus&) noexcept->Editable*;
            // create this
            static auto CreateSP(const EditableStatus& es) noexcept {
                return RefPtr<Drawable::Editable>(Editable::Create(es));
            }
        protected:
            // ctor
            Editable(const EditableStatus&) noexcept;
            // ctor
            Editable(const Editable&) = delete;
            // ctor
            ~Editable() noexcept;
        protected:
            // buffer
            size_t              m_buffer[0];
        };*/
    }
}