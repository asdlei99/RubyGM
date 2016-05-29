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

// C/C++
#include <cstdint>
#include <utility>
#include <cassert>
// RubyGM
#include "rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include "../asset/rgmAsset.h"

// rubygm namespace
namespace RubyGM {
    // resource namespace
    namespace Drawable {
        // status for bitmap
        struct BitmapStatus : BaseStatus {
            // bitmap asset, CANNOT be null
            RefPtr<Asset::Bitmap>   bitmap;
            // src clip
            RectF                   src;
            // display rect
            RectF                   des;
            // default value
            inline BitmapStatus(const RefPtr<Asset::Bitmap> & b) : 
                BaseStatus(), bitmap(b) 
            { src = { 0.f }; des = { 0.f }; asset_check(); }
            // default value
            inline BitmapStatus(RefPtr<Asset::Bitmap>&& b) : 
                BaseStatus(), bitmap(std::move(b))
            { src = { 0.f }; des = { 0.f }; asset_check();}
        private:
            // asset check
            inline void asset_check() { 
                assert(bitmap && "bitmap asset cannot be null");
            }
        };
        // Status for PerspectiveBitmap
        struct PerspectiveBitmapStatus : BitmapStatus {
            // default value
            inline PerspectiveBitmapStatus(const RefPtr<Asset::Bitmap>& b) : 
                BitmapStatus(b) {}
            // default value
            inline PerspectiveBitmapStatus(RefPtr<Asset::Bitmap>&& b) :
                BitmapStatus(std::move(b)) {}
        };
        // drawable bitmap 
        class Bitmap : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
        public:
            // create this
            static auto Create(const BitmapStatus&) noexcept->Bitmap*;
            // create this
            static auto CreateSP(const BitmapStatus& bs) noexcept {
                return std::move(RubyGM::RefPtr<Drawable::Bitmap>(
                    std::move(Bitmap::Create(bs)))
                );
            }
        protected:
            // ctor
            Bitmap(const BitmapStatus&) noexcept;
            // ctor
            Bitmap(const Bitmap&) = delete;
            // dtor
            ~Bitmap() noexcept;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
            // get bitmap, won't add refcount
            auto&RefBitmap() const noexcept { return m_spAsBitmap; }
            // get bitmap, will add refcount
            auto GetBitmap() const noexcept { return m_spAsBitmap; }
        protected:
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        private:
            // dispose object
            void dispose() noexcept override;
            // reset bitmap size
            void reset_bitmap_size() noexcept;
        public:
            // get width
            auto GetWidth() const noexcept { return m_fWidth; }
            // get height
            auto GetHeight() const noexcept { return m_fHeight; }
            // get interpolation mode
            auto GetInterpolationMode() const noexcept { return m_modeInter; }
            // set interpolation mode
            void SetInterpolationMode(InterpolationMode mode) noexcept;
        protected:
            // bitmap asset
            RefPtr<Asset::Bitmap>   m_spAsBitmap;
            // bitmap graphics interface data
            IGMBitmap*              m_pGiBitmap = nullptr;
            // width of bitmap
            float                   m_fWidth = 1.f;
            // height of bitmap
            float                   m_fHeight = 1.f;
            // interpolation mode
            InterpolationMode       m_modeInter = Mode_Linear;
        public:
            // opacity
            float                   opacity = 1.f;
            // src clip rect
            RectF                   src_rect{ 0.f };
            // display rect
            RectF                   des_rect{ 0.f };
        };
        // drawable perspective-bitmap 
        class PerspectiveBitmap : public Drawable::Bitmap {
            // super class
            using Super = Drawable::Bitmap;
        public:
            // create this
            static auto Create(const PerspectiveBitmapStatus&) 
                noexcept->PerspectiveBitmap*;
            // create this
            static auto CreateSP(const PerspectiveBitmapStatus& bs) noexcept {
                return std::move(RubyGM::RefPtr<Drawable::PerspectiveBitmap>(
                    std::move(PerspectiveBitmap::Create(bs)))
                );
            }
        protected:
            // ctor
            PerspectiveBitmap(const PerspectiveBitmapStatus&) noexcept;
            // ctor
            PerspectiveBitmap(const Bitmap&) = delete;
            // dtor
            ~PerspectiveBitmap() noexcept {}
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        private:
            // dispose object
            void dispose() noexcept override;
        public:
            // perspective matrix
            Matrix4X4F          perspective;
        };
    }
}