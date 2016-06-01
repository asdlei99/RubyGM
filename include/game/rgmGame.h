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
#include "../core/util/rgmUtil.h"
#include "../core/util/rgmStruct.h"
#include "../core/asset/rgmAsset.h"
#include "../core/asset/rgmAssetFont.h"
#include "../core/asset/rgmAssetStroke.h"

// rubygm namespace
namespace RubyGM {
    // max resource each
    enum : size_t { MAX_SOURCE_EACH = 256, };
    // sprite class
    class CGMSprite;
    // sprte status
    struct SprteStatus;
    // prop for font
    struct FontProperties;
    // prop for linear gradient brush
    struct LinearBrush;
    // prop for radial gradient brush
    struct RadialBrush;
    // StrokeStyle
    struct StrokeStyle;
    // asset bitmap
    using AssetBitmap = RefPtr<Asset::Bitmap>;
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
    // get bitmap resource by index, will add ref-count
    auto GetBitmapAsset(uint32_t index) noexcept -> RefPtr<Asset::Bitmap>;
    // get font resource by index, will add ref-count
    auto GetFontAsset(uint32_t index) noexcept -> RefPtr<Asset::Font>;
    // get brush resource by index, will add ref-count
    auto GetBrushAsset(uint32_t index) noexcept -> RefPtr<Asset::Brush>;
    // ------------------------------------------------------------------------
    // ------------------------------- CREATE ---------------------------------
    // ------------------------------------------------------------------------
    // creat font resource
    auto CreateFontAsset(const FontProperties&) noexcept ->RefPtr<Asset::Font>;
    // creat stroke resource
    auto CreateStrokeAsset(const StrokeStyle&)noexcept ->RefPtr<Asset::Stroke>;
    // create solid color brush resource
    auto CreateBrushAsset(const ColorF&) noexcept ->RefPtr<Asset::Brush>;
    // create linear gradient brush resource
    auto CreateBrushAsset(const LinearBrush&) noexcept ->RefPtr<Asset::Brush>;
    // create radial gradient brush resource
    auto CreateBrushAsset(const RadialBrush&) noexcept ->RefPtr<Asset::Brush>;
    // create radial gradient brush resource
    auto CreateBrushAsset(const AssetBitmap&) noexcept ->RefPtr<Asset::Brush>;
    // create bitmap asset from drawable in zoomed size
    auto CreateBitmapAssetFromDrawable(Drawable::Object*, 
        SizeF sf, SizeF bs) noexcept-> RefPtr<Asset::Bitmap>;
    // create bitmap asset from file name
    auto CreateBitmapAssetFromFile(const wchar_t* namebgn, 
        const wchar_t* nameend) noexcept->RefPtr<Asset::Bitmap>;
    // create bitmap asser from file name
    inline auto CreateBitmapAssetFromFile(
        const wchar_t* name) noexcept->RefPtr<Asset::Bitmap> {
        auto end = name + std::wcslen(name);
        return Game::CreateBitmapAssetFromFile(name, end);
    }
    // ------------------------------------------------------------------------
    // ------------------------------ REGISTER --------------------------------
    // --------- if out of range(MAX_SOURCE_EACH), do nothing, return 0 -------
    // ------------------------------------------------------------------------
    // register a new bitmap resource, return index, 
    auto RegisterBitmapAsset() noexcept -> uint32_t;
    // register a new brush resource, return index, 
    auto RegisterBrushAsset(Asset::Brush&) noexcept -> uint32_t;
    // register a new font resource, return index, 
    auto RegisterFontAsset(Asset::Font&) noexcept -> uint32_t;
    // register helper
    /*inline auto RegisterAsset(Asset::Font& f) noexcept {
        return RegisterFontAsset(f);
    }
    // register helper
    inline auto RegisterAsset(Asset::Brush& b) noexcept {
        return RegisterBrushAsset(b);
    }*/
}}

