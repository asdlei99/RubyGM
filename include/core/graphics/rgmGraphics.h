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

// graphics
#include <d2d1_3.h>

// rubygm namespace
namespace RubyGM {
    // stroke style
    struct IGMStrokeStyle : ID2D1StrokeStyle { using Super = ID2D1StrokeStyle; };
    // render context
    struct IGMRenderContext : ID2D1DeviceContext3 { using Super = ID2D1DeviceContext3; };
    // bitmap
    struct IGMBitmap : ID2D1Bitmap1 { using Super = ID2D1Bitmap1; };
    // geometry
    struct IGMGeometry : ID2D1Geometry { using Super = ID2D1Geometry; };
    // path geometry
    struct IGMPath : ID2D1PathGeometry { using Super = ID2D1PathGeometry; };
    // brush
    struct IGMBrush : ID2D1Brush { using Super = ID2D1Brush; };
}
