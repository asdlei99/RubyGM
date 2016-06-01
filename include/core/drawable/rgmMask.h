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
        // status for mask
        struct MaskStatus : BaseStatus {
            // mask bitmap asset, CANNOT be null
            RefPtr<Asset::Bitmap>   mask;
            // filled brush asset , CANNOT be null
            RefPtr<Asset::Brush>    brush;
            // default value
            inline MaskStatus(const RefPtr<Asset::Bitmap>& m,
                const RefPtr<Asset::Brush>& b) : 
                BaseStatus(), mask(m), brush(b) { asset_check(); }
            // default value
            inline MaskStatus(RefPtr<Asset::Bitmap>&& m,
                RefPtr<Asset::Brush>&& b) : BaseStatus(),
                 mask(std::move(m)), brush(std::move(b)) { asset_check(); }
        private:
            // asset check
            inline void asset_check() { 
                assert(mask && "mask bitmap asset cannot be null");
                assert(brush && "filled brush asset cannot be null");
            }
        };
        // drawable bitmap 
        class Mask : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
        public:
            // create this
            static auto Create(const MaskStatus&) noexcept->Mask*;
            // create this
            static auto CreateSP(const MaskStatus& bs) noexcept {
                return RefPtr<Drawable::Mask>(Mask::Create(bs));
            }
        private:
            // ctor
            Mask(const MaskStatus&) noexcept;
            // ctor
            Mask(const Mask&) = delete;
            // dtor
            ~Mask() noexcept;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        protected:
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        private:
            // dispose object
            void dispose() noexcept override;
        protected:
            // mask bitmap
            RefPtr<Asset::Bitmap>   m_spAsMask;
            // filled brush
            RefPtr<Asset::Brush>    m_spAsBrush;
            // mask graphics interface data
            IGMBitmap*              m_pGiMask = nullptr;
            // filled brush graphics interface data
            IGMBrush*               m_pGiBrush = nullptr;
        };
    }
}