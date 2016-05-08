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
#include <cassert>
#include <new>

#pragma warning(disable: 4200)
// rubygm namespace
namespace RubyGM {
#pragma pack(push)
#pragma pack(1)
    // raw titled map data
    struct RawTitledMap {
        // this data length
        uint32_t        data_length;
        // reserved data
        uint32_t        data_reserved;
        // width of tile
        uint16_t        tile_width;
        // height of tile
        uint16_t        tile_height;
        // width of map
        uint16_t        map_width;
        // height of map
        uint16_t        map_height;
        // map real data
        uint8_t         map_data[0];
    };
#pragma pack(pop)
    // point
    template<typename T> struct Point { T x; T y; };
    // rect
    template<typename T> struct Rect { T left; T top; T right; T bottom; };
    // size
    template<typename T> struct Size { T width; T height; };
    // Point2U
    using Point2U = Point<uint32_t>;
    // Point2F
    using Point2F = Point<float>;
    // Point2L
    using Point2L = Point<int32_t>;
    // Vector2F
    struct Vector2F { float x; float y; };
    // Vector3F
    struct Vector3F { float x; float y; float z; };
    // Vector4F
    struct Vector4F { float x; float y; float z; float w;};
    // RectF
    using RectF = Rect<float>;
    // RectU
    using RectU = Rect<uint32_t>;
    // RectL
    using RectL = Rect<int32_t>;
    // SizeF
    using SizeF = Size<float>;
    // SizeU
    using SizeU = Size<uint32_t>;
    // ColorF
    struct ColorF { float r; float g; float b; float a; };
    // Matrix3X2F
    struct Matrix3X2F { float _11, _12, _21, _22, _31, _32; };
    // Ellipse
    struct Ellipse { Point2F point; float radius_x; float radius_y; };
}