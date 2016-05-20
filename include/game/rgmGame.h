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
#include <cwchar>
#include "../rubygm.h"
#include "../core/util/rgmStruct.h"

// rubygm namespace
namespace RubyGM {
    // max resource each
    enum : size_t { MAX_SOURCE_EACH = 1024, };
    // sprite class
    class CGMSprite;
    // sprte status
    struct SprteStatus;
    // prop for font
    struct FontProperties;
    // prop for nromal brush
    struct NBrushProperties;
}

// rubygm::base namespace
namespace RubyGM { namespace Base {
    // resource class
    class Resource;
}}

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    // object
    class Object;
}}


// rubygm::resource namespace
namespace RubyGM { namespace Asset {
    // object, font, bitmap and bursh class
    struct Object; class Font; struct Bitmap; struct Brush;
}}

// rubygm::game namespace
namespace RubyGM { namespace Game {
    // reset random seed, read from OS
    //void ResetRandomSeed() noexcept;
    // get a random data
    //auto Random() noexcept -> float;
    // get delta time
    auto GetDetalTime() noexcept -> float;
    // get time scale
    auto GetTimeScale() noexcept -> float;
    // set time scale
    void SetTimeScale(float ts) noexcept;
    // set last error code
    void SetLastErrorCode(Result code) noexcept;
    // get last error code
    auto GetLastErrorCode() noexcept->Result;
    // add s sprite to game list
    auto NewSprite(const SprteStatus&) noexcept ->CGMSprite*;
    // ------------------------------------------------------------------------
    // -------------------------------- GET -----------------------------------
    // ------------------------------------------------------------------------
    // get bitmap resource by index, won't add ref-count
    auto RefBitmapAsset(uint32_t index) noexcept -> Asset::Bitmap&;
    // get font resource by index, won't add ref-count
    auto RefFontAsset(uint32_t index) noexcept -> Asset::Font&;
    // get brush resource by index, won't add ref-count
    auto RefBrushAsset(uint32_t index) noexcept -> Asset::Brush&;

    // get bitmap resource by index, will add ref-count
    auto GetBitmapAsset(uint32_t index) noexcept -> Asset::Bitmap&;
    // get font resource by index, will add ref-count
    auto GetFontAsset(uint32_t index) noexcept -> Asset::Font&;
    // get brush resource by index, will add ref-count
    auto GetBrushAsset(uint32_t index) noexcept -> Asset::Brush&;
    // ------------------------------------------------------------------------
    // ------------------------------- CREATE ---------------------------------
    // ------------------------------------------------------------------------
    // creat font resource, ref-count set -> 1
    auto CreateFontAsset(const FontProperties&) noexcept -> Asset::Font&;
    // create normal brush resource, ref-count set -> 1
    auto CreateBrushAsset(const NBrushProperties&) noexcept -> Asset::Brush&;
    // create bitmap asset from drawable in zoomed size
    auto CreateBitmapAssetFromDrawable(Drawable::Object*, 
        SizeF sf, SizeF bs) noexcept->Asset::Bitmap&;
    // create bitmap asset from file name
    auto CreateBitmapAssetFromFile(const wchar_t* namebgn, 
        const wchar_t* nameend) noexcept->Asset::Bitmap&;
    // create bitmap asser from file name
    inline auto CreateBitmapAssetFromFile(
        const wchar_t* name) noexcept->Asset::Bitmap& {
        return Game::CreateBitmapAssetFromFile(name, name + std::wcslen(name));
    }
    // ------------------------------------------------------------------------
    // ------------------------------ REGISTER --------------------------------
    // --------- if out of range(MAX_SOURCE_EACH), do nothing, return 0 -------
    // ------------------------------------------------------------------------
    // register a new bitmap resource, return index, 
    auto RegisterBitmapAsset() noexcept -> uint32_t;
    // register a new brush resource, return index, 
    auto RegisterBrushAsset(const NBrushProperties&) noexcept -> uint32_t;
    // register a new font resource, return index, 
    auto RegisterFontAsset(const FontProperties&) noexcept -> uint32_t;
}}

