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
    // batch
    struct IGMBatch;
    // resource namespace
    namespace Drawable {
        // status for batch
        struct BatchStatus : BaseStatus {
            // bitmap asset, CANNOT be null
            RefPtr<Asset::Bitmap>   bitmap;
            // default value
            inline BatchStatus(const RefPtr<Asset::Bitmap> & b) : 
                BaseStatus(), bitmap(b) { asset_check(); }
            // default value
            inline BatchStatus(RefPtr<Asset::Bitmap>&& b) : 
                BaseStatus(), bitmap(std::move(b)) { asset_check();}
        private:
            // asset check
            inline void asset_check() { 
                assert(bitmap && "bitmap asset cannot be null");
            }
        };
        // drawable bitmap 
        class Batch : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
            // dispose object
            void dispose() noexcept override;
            // reset batch
            void reset_batch() noexcept;
        public:
            // create this
            static auto Create(const BatchStatus&) noexcept->Batch*;
            // create this
            static auto CreateSP(const BatchStatus& bs) noexcept {
                return std::move(RubyGM::RefPtr<Drawable::Batch>(
                    std::move(Batch::Create(bs)))
                );
            }
        protected:
            // ctor
            Batch(const BatchStatus&) noexcept;
            // ctor
            Batch(const Batch&) = delete;
            // dtor
            ~Batch() noexcept;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
            // set the interpolation mode
            void SetInterpolationMode(InterpolationMode) noexcept;
        protected:
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        protected:
            // batch bitmap
            RefPtr<Asset::Bitmap>   m_spAsBitmap;
            // batch bitmap graphics interface
            IGMBitmap*              m_pGiBitmap = nullptr;
            // batch graphics interface
            IGMBatch*               m_pGiBatch = nullptr;
            // unit total count
            uint32_t                m_cUnitTotal = 0;
            // unit display start index
            uint32_t                m_iUnitStart = 0;
            // unit display count
            uint32_t                m_cUnitDisplay = 0;
            // interpolation mode
            InterpolationMode       m_modeInter = Mode_Linear;
        };
    }
}