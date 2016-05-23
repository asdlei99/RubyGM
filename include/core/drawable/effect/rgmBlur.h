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

#include "rgmEffect.h"
#include "../asset/rgmAsset.h"

// rubygm namespace
namespace RubyGM {
    // resource namespace
    namespace Drawable {
        // status for GaussianBlur
        struct GaussianBlurStatus : EffectStatus {
            // source bitmap
            Asset::Bitmap&      bitmap;
            // StandardDeviation
            float               sd;
            // ctor
            ~GaussianBlurStatus() noexcept { bitmap.Release(); }
            // default ctor
            inline GaussianBlurStatus(Asset::Bitmap&& b) : bitmap(b) { }
            // default value
            inline GaussianBlurStatus(Asset::Bitmap&& b, Default v) :
                EffectStatus(v), bitmap(b) {
                sd = 3.0f;
            }
        };
        // gaussian blur effect
        class GaussianBlur final : public Drawable::Effect {
            // super class
            using Super = Drawable::Effect;
            // helper
            friend EffectHelper<GaussianBlur>;
            // dispose
            void dispose() noexcept override;
        public:
            // create this
            static auto Create(const GaussianBlurStatus&) 
                noexcept->GaussianBlur*;
            // create this
            static auto CreateSP(const GaussianBlurStatus& ts) noexcept {
                return std::move(RubyGM::CGMPtrA<Drawable::GaussianBlur>(
                    std::move(GaussianBlur::Create(ts)))
                );
            }
        public:
            // set StandardDeviation
            auto SetStandardDeviation(float sd) noexcept ->Result;
            // get StandardDeviation
            auto GetStandardDeviation() const noexcept { return m_fStdDvi; }
        protected:
            // ctor
            GaussianBlur(const GaussianBlurStatus&) noexcept;
            // ctor
            GaussianBlur(const GaussianBlur&) = delete;
            // GaussianBlur
            ~GaussianBlur() noexcept;
        protected:
            // init
            void init() noexcept;
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        protected:
            // bitmap asset
            Asset::Bitmap&              m_refBitmapAsset;
            // blur Standard Deviation
            float                       m_fStdDvi = -1.f;
        };
    }
}