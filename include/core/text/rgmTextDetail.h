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

#include <LongUI.h>
#include <LongUI/luiUiTxtRdr.h>
#include <dwrite_1.h>

// rubygm namespace
namespace RubyGM { 
    // font
    struct IGMFont : IDWriteTextFormat { using Super = IDWriteTextFormat; };
    // text layout
    struct IGMTextlayout : IDWriteTextLayout { using Super = IDWriteTextLayout; };
    // text renderer
    struct IGMTextRenderer : LongUI::XUIBasicTextRenderer { using Super = LongUI::XUIBasicTextRenderer; };
}

// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // TextMetrics
    inline auto&d2d(TextMetrics& tm) { return reinterpret_cast<DWRITE_TEXT_METRICS&>(tm); }
    // TextRenderer
    inline auto rubygm(LongUI::XUIBasicTextRenderer* r) { return reinterpret_cast<IGMTextRenderer*>(r); }
}}
