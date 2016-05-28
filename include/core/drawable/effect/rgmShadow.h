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
        // status for shadow
        struct ShadowStatus : EffectStatus {
            // bitmap asset, CANNOT be null
            RefPtr<Asset::Bitmap>   bitmap;
            // color
            ColorF                  color;
            // StandardDeviation for blue
            float                   blur_sd;
            // default value
            inline ShadowStatus(const RefPtr<Asset::Bitmap>& b) : 
                EffectStatus(), bitmap(b)  {
                color.r = color.g = color.b = 0.f; color.a = 1.f; 
                blur_sd = 3.f; asset_check();
            }
            // default value
            inline ShadowStatus(RefPtr<Asset::Bitmap>&& b) : 
                EffectStatus(), bitmap(std::move(b)) {
                color.r = color.g = color.b = 0.f; color.a = 1.f; 
                blur_sd = 3.f; asset_check();
            }
        private:
            // asset check
            inline void asset_check() { 
                assert(bitmap && "bitmap asset cannot be null");
            }
        };
        // gaussian blur effect
        class Shadow final : public Drawable::Effect {
            // super class
            using Super = Drawable::Effect;
            // dispose
            void dispose() noexcept override;
        public:
            // create this
            static auto Create(const ShadowStatus&) noexcept->Shadow*;
            // create this
            static auto CreateSP(const ShadowStatus& ts) noexcept {
                return std::move(RubyGM::RefPtr<Drawable::Shadow>(
                    std::move(Shadow::Create(ts)))
                );
            }
        public:
            // set blur color
            auto SetColor(const ColorF&) noexcept ->Result;
            // get blur StandardDeviation
            auto GetColor(ColorF& color) const noexcept { color = m_color; }
            // set blur StandardDeviation
            auto SetStandardDeviation(float sd) noexcept ->Result;
            // get blur StandardDeviation
            auto GetStandardDeviation() const noexcept { return m_fStdDvi; }
        protected:
            // ctor
            Shadow(const ShadowStatus&) noexcept;
            // ctor
            Shadow(const Shadow&) = delete;
            // GaussianBlur
            ~Shadow() noexcept;
        protected:
            // init
            void init() noexcept;
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        protected:
            // bitmap asset
            RefPtr<Asset::Bitmap>       m_spAsBitmap;
            // bitmap graphics interface
            // color
            ColorF                      m_color;
            // StandardDeviation
            float                       m_fStdDvi = -1.f;
        };
    }
}