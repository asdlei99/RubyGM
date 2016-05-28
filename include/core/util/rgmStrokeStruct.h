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

// rubygm namespace
namespace RubyGM { 
    // cap style
    enum CapStyle : uint8_t {
        // Flat line cap.
        Style_Flat = 0,
        // Square line cap.
        Style_Square = 1,
        // Round line cap.
        Style_Round = 2,
        // Triangle line cap.
        Style_Triangle = 3,
    };
    // line join
    enum LineJoin : uint8_t {
        // Miter join.
        Join_Miter = 0,
        // Bevel join.
        Join_Bevel = 1,
        // Round join.
        Join_Round = 2,
        // Miter/Bevel join.
        Join_MiterOrBevel = 3,
    };
    // basic struct
    struct BaseStrokeStyle {
        // count of dash array
        uint32_t        dash_count;
        // >= 1.0, miterLength / stroke-width = 1 / sin ( theta / 2 )
        // line join--------------------like in svg[stroke-linejoin]
        LineJoin        line_join;
        // cap style for beginning------like in svg[stroke-linecap]
        CapStyle        cap_begin;
        // cap style for ending---------like in svg[stroke-linecap]
        CapStyle        cap_end;
        // cap style for dash-line------like in svg[stroke-linecap]
        CapStyle        cap_dash;
        // miter limit------------------like in svg[stroke-miterlimit]
        float           miter_limit;
        // dash offset------------------like in svg[stroke-dashoffset]
        float           dash_offset;
    };
    // aside from the width, that allow geometric penning to be specified.
    struct StrokeStyle : BaseStrokeStyle {
        // dash array-------------------like in svg[stroke-dasharray]
        float*          dashes;
    };
}

