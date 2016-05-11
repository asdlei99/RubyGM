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
#include "../util/rgmStruct.h"

// rubygm namespace
namespace RubyGM { 
    // brush type
    enum BrushType:uint32_t{ Type_Color,Type_Linear,Type_Radial,Type_Other };
    // brush
    struct IGMBrush;
    // properties to creating normal brush
    struct NBrushProperties {
        // brush type
        BrushType           type;
        // brush opacity
        float               opacity;
        // union
        union {
            //  color
            ColorF          color;
        };
    };
    // make color brush
    inline auto MakeColorBrush(
        NBrushProperties& prop,
        const RubyGM::ColorF& c,
        float opcity
    )->NBrushProperties& {
        prop.opacity = opcity;
        prop.type = BrushType::Type_Color;
        prop.color = c;
        return prop;
    }
    // make color brush
    inline auto MakeColorBrush(
        NBrushProperties& prop,
        const RubyGM::ColorF& c
    )->NBrushProperties& {
        return MakeColorBrush(prop, c, 1.f);
    }
}

