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
#pragma warning(disable: 4200)

// rubygm namespace
namespace RubyGM { 
    // font
    struct IGMFont;
    // text range
    struct TextRange { uint32_t begin; uint32_t length; };
    // text metrics
    struct TextMetrics {
        float left, top, width, width_ex, height, layout_width, layout_height;
        uint32_t    max_depth, line_count;
    };
    // properties to creating font
    struct FontProperties {
        // text size
        float           size;
        // tab width, 0.f for default(size x 4)
        float           tab;
        // weight 0~1000 in uint16_t
        uint16_t        weight;
        // style 0~x in uint8_t
        uint8_t         style;
        // stretch 0~x in uint8_t
        uint8_t         stretch;
        // valign 0~x in uint8_t
        uint8_t         valign;
        // halign 0~x in uint8_t
        uint8_t         halign;
        // flow direction 0~x in uint8_t
        uint8_t         flow;
        // reading direction 0~x in uint8_t
        uint8_t         reading;
        // word wrapping
        uint32_t        wrapping;
        // font name, with null-termxxx character
        wchar_t         name[0];
    };
}

