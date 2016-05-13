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
        float left, top, width, width_with_trailing_whitespace, height;
        float layout_width, layout_height;
        uint32_t    max_depth, line_count;
    };
    // line metrics
    struct LineMetrics {
        uint32_t len;
        uint32_t trailing_whitespace_length;
        uint32_t newline_len;
        float    height;
        float    baseline;
        int32_t  is_trimmed;
    };
    // hittest metrics
    struct HittestMetrics {
        uint32_t pos, len;
        float left, top, width, height;
        uint32_t bidi_level;
        int32_t is_text, is_trimmed;
    };
    // font weight
    enum FontWeight : uint16_t;
    // font style
    enum FontStyle : uint8_t;
    // font stretch
    enum FontStretch : uint8_t;
    // alignment in vertical direction
    enum VAlignment : uint8_t;
    // alignment in horizontal direction
    enum HAlignment : uint8_t;
    // flow direction
    enum FlowDirection : uint8_t;
    // reading direction
    enum ReadDirection  : uint8_t;
    // word wrapping
    enum WordWrapping  : uint32_t;
    // properties to creating font
    struct FontProperties {
        // text size
        float           size;
        // tab width, 0.f for default(size x 4)
        float           tab;
        // weight 0~1000 in uint16_t
        FontWeight      weight;
        // style 0~x in uint8_t
        FontStyle       style;
        // stretch 0~x in uint8_t
        FontStretch     stretch;
        // valign 0~x in uint8_t
        VAlignment      valign;
        // halign 0~x in uint8_t
        HAlignment      halign;
        // flow direction 0~x in uint8_t
        FlowDirection   flow;
        // reading direction 0~x in uint8_t
        ReadDirection   reading;
        // word wrapping
        WordWrapping    wrapping;
        // font name, with null-termxxx character
        wchar_t         name[0];
    };
    // font weight
    enum FontWeight : uint16_t  {
        /// <summary>
        /// Predefined font weight : Thin (100).
        /// </summary>
        Weight_Thin = 100,
        /// <summary>
        /// Predefined font weight : Extra-light (200).
        /// </summary>
        Weight_ExtraLight = 200,
        /// <summary>
        /// Predefined font weight : Ultra-light (200).
        /// </summary>
        Weight_UltraLight = 200,
        /// <summary>
        /// Predefined font weight : Light (300).
        /// </summary>
        Weight_LighT = 300,
        /// <summary>
        /// Predefined font weight : Semi-light (350).
        /// </summary>
        Weight_SemiLighT = 350,
        /// <summary>
        /// Predefined font weight : Normal (400).
        /// </summary>
        Weight_Normal = 400,
        /// <summary>
        /// Predefined font weight : Regular (400).
        /// </summary>
        Weight_Regular = 400,
        /// <summary>
        /// Predefined font weight : Medium (500).
        /// </summary>
        Weight_Medium = 500,
        /// <summary>
        /// Predefined font weight : Demi-bold (600).
        /// </summary>
        Weight_DemiBold = 600,
        /// <summary>
        /// Predefined font weight : Semi-bold (600).
        /// </summary>
        Weight_SemiBold = 600,
        /// <summary>
        /// Predefined font weight : Bold (700).
        /// </summary>
        Weight_Bold = 700,
        /// <summary>
        /// Predefined font weight : Extra-bold (800).
        /// </summary>
        Weight_ExtraBold = 800,
        /// <summary>
        /// Predefined font weight : Ultra-bold (800).
        /// </summary>
        Weight_UltraBold = 800,
        /// <summary>
        /// Predefined font weight : Black (900).
        /// </summary>
        Weight_Black = 900,
        /// <summary>
        /// Predefined font weight : Heavy (900).
        /// </summary>
        Weight_Heavy = 900,
        /// <summary>
        /// Predefined font weight : Extra-black (950).
        /// </summary>
        Weight_ExtraBlack = 950,
        /// <summary>
        /// Predefined font weight : Ultra-black (950).
        /// </summary>
        Weight_UltraBlack = 950
    };
    // font style
    enum FontStyle : uint8_t {
        /// <summary>
        /// Font slope style : Normal.
        /// </summary>
        Style_Normal = 0,
        /// <summary>
        /// Font slope style : Oblique.
        /// </summary>
        Style_Oblique,
        /// <summary>
        /// Font slope style : Italic.
        /// </summary>
        Style_Italic
    };
    // font stretch
    enum FontStretch : uint8_t {
        /// <summary>
        /// Predefined font stretch : Not known (0).
        /// </summary>
        Stretch_Undefined = 0,
        /// <summary>
        /// Predefined font stretch : Ultra-condensed (1).
        /// </summary>
        Stretch_UltraCondensed = 1,
        /// <summary>
        /// Predefined font stretch : Extra-condensed (2).
        /// </summary>
        Stretch_ExtraCondensed = 2,
        /// <summary>
        /// Predefined font stretch : Condensed (3).
        /// </summary>
        Stretch_Condensed = 3,
        /// <summary>
        /// Predefined font stretch : Semi-condensed (4).
        /// </summary>
        Stretch_SemiEondensed = 4,
        /// <summary>
        /// Predefined font stretch : Normal (5).
        /// </summary>
        Stretch_Normal = 5,
        /// <summary>
        /// Predefined font stretch : Medium (5).
        /// </summary>
        Stretch_Medium = 5,
        /// <summary>
        /// Predefined font stretch : Semi-expanded (6).
        /// </summary>
        Stretch_SemiExpanded = 6,
        /// <summary>
        /// Predefined font stretch : Expanded (7).
        /// </summary>
        Stretch_Expanded = 7,
        /// <summary>
        /// Predefined font stretch : Extra-expanded (8).
        /// </summary>
        Stretch_ExtraExpanded = 8,
        /// <summary>
        /// Predefined font stretch : Ultra-expanded (9).
        /// </summary>
        Stretch_UltraExpanded = 9
    };
    // alignment in vertical direction
    enum VAlignment : uint8_t {
        /// <summary>
        /// The first line of paragraph is aligned to the flow's beginning edge of the layout box.
        /// </summary>
        VAlignment_Near,
        /// <summary>
        /// The last line of paragraph is aligned to the flow's ending edge of the layout box.
        /// </summary>
        VAlignment_Far,
        /// <summary>
        /// The center of the paragraph is aligned to the center of the flow of the layout box.
        /// </summary>
        VAlignment_Center
    };
    // alignment in horizontal direction
    enum HAlignment : uint8_t {
        /// <summary>
        /// The leading edge of the paragraph text is aligned to the layout box's leading edge.
        /// </summary>
        HAlignment_Leading,
        /// <summary>
        /// The trailing edge of the paragraph text is aligned to the layout box's trailing edge.
        /// </summary>
        HAlignment_Trailing,
        /// <summary>
        /// The center of the paragraph text is aligned to the center of the layout box.
        /// </summary>
        HAlignment_Center,
        /// <summary>
        /// Align text to the leading side, and also justify text to fill the lines.
        /// </summary>
        HAlignment_Justified
    };
    // flow direction
    enum FlowDirection : uint8_t    {
        /// <summary>
        /// Text lines are placed from top to bottom.
        /// </summary>
       FDirection_Top2Bottom = 0,
        /// <summary>
        /// Text lines are placed from bottom to top.
        /// </summary>
       FDirection_Bottom2Top = 1,
        /// <summary>
        /// Text lines are placed from left to right.
        /// </summary>
       FDirection_Left2Right = 2,
        /// <summary>
        /// Text lines are placed from right to left.
        /// </summary>
       FDirection_Right2Left = 3,
    };
    // reading direction
    enum ReadDirection : uint8_t {
        /// <summary>
        /// Reading progresses from left to right.
        /// </summary>
        RDirection_Left2Right = 0,
        /// <summary>
        /// Reading progresses from right to left.
        /// </summary>
        RDirection_Right2Left = 1,
        /// <summary>
        /// Reading progresses from top to bottom.
        /// </summary>
        RDirection_Top2Bottom = 2,
        /// <summary>
        /// Reading progresses from bottom to top.
        /// </summary>
        RDirection_Bottom2Top = 3,
    };
    // word wrapping
    enum WordWrapping : uint32_t {
        /// <summary>
        /// Words are broken across lines to avoid text overflowing the layout box.
        /// </summary>
        Wrapping_Wrap = 0,
        /// <summary>
        /// Words are kept within the same line even when it overflows the layout box.
        /// This option is often used with scrolling to reveal overflow text. 
        /// </summary>
        Wrapping_NoWrap = 1,
        /// <summary>
        /// Words are broken across lines to avoid text overflowing the layout box.
        /// Emergency wrapping occurs if the word is larger than the maximum width.
        /// </summary>
        Wrapping_EmergencyBreak = 2,
        /// <summary>
        /// Only wrap whole words, never breaking words (emergency wrapping) when the
        /// layout width is too small for even a single word.
        /// </summary>
        Wrapping_WholeWord = 3,
        /// <summary>
        /// Wrap between any valid characters clusters.
        /// </summary>
        Wrapping_Character = 4,
    };
}

