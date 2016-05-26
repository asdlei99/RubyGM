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
#include "rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include "../asset/rgmAsset.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // resource namespace
    namespace Drawable {
        // status for batch
        struct BatchStatus : BaseStatus {
            // batch bitmap asset
            Asset::Bitmap&      batch;
            // ctor
            ~BatchStatus() noexcept { batch.Release(); }
            // default ctor
            inline BatchStatus(Asset::Bitmap&& b) : batch(b) { }
            // default value
            inline BatchStatus(Asset::Bitmap&& b, Default v) : 
                BaseStatus(v), batch(b) {
            }
        };
        // drawable bitmap 
        class Batch : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
        public:
            // create this
            static auto Create(const BatchStatus&) noexcept->Batch*;
            // create this
            static auto CreateSP(const BatchStatus& bs) noexcept {
                return std::move(RubyGM::CGMPtrA<Drawable::Batch>(
                    std::move(Batch::Create(bs)))
                );
            }
        private:
            // ctor
            Batch(const BatchStatus&) noexcept;
            // ctor
            Batch(const Batch&) = delete;
            // dtor
            ~Batch() noexcept;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        protected:
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        private:
            // dispose object
            void dispose() noexcept override;
            // reset bitmap size
            void reset_bitmap_size() noexcept;
        public:
            // save as png file, will save total file
            auto SaveAsPng(const wchar_t* file_name) noexcept ->Result;
            // save as png file with utf8
            auto SaveAsPng(const char* file_name) noexcept ->Result;
            // get width
            auto GetWidth() const noexcept { return m_fWidth; }
            // get height
            auto GetHeight() const noexcept { return m_fHeight; }
            // get interpolation mode
            auto GetInterpolationMode() const noexcept { return m_modeInter; }
            // set interpolation mode
            void SetInterpolationMode(InterpolationMode mode) noexcept;
        private:
            // batch bitmap asset
            Asset::Bitmap&      m_refBatch;
            // bitmap data
            IGMBitmap*          m_pBatch = nullptr;
            // width of bitmap
            float               m_fWidth = 1.f;
            // height of bitmap
            float               m_fHeight = 1.f;
            // interpolation mode
            InterpolationMode   m_modeInter;
        public:
            // opacity
            float               opacity = 1.f;
            // src clip rect
            RectF               src_rect{ 0.f };
            // display rect
            RectF               des_rect{ 0.f };
        };
    }
}