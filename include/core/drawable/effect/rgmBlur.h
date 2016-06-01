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
            // bitmap asset, CANNOT be null
            RefPtr<Asset::Bitmap>   bitmap;
            // StandardDeviation
            float                   sd;
            // default value
            inline GaussianBlurStatus(const RefPtr<Asset::Bitmap>& b) : 
                EffectStatus(), bitmap(b)  {
                sd = 3.f; asset_check();
            }
            // default value
            inline GaussianBlurStatus(RefPtr<Asset::Bitmap>&& b) : 
                EffectStatus(), bitmap(std::move(b)) {
                sd = 3.f; asset_check();
            }
        private:
            // asset check
            inline void asset_check() { 
                assert(bitmap && "bitmap asset cannot be null");
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
                return RefPtr<Drawable::GaussianBlur>(GaussianBlur::Create(ts));
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
            RefPtr<Asset::Bitmap>       m_spAsBitmap;
            // blur Standard Deviation
            float                       m_fStdDvi = -1.f;
        };
        // status for DirectionalBlur
        /*struct DirectionalBlurStatus : EffectStatus {
            // source bitmap
            Asset::Bitmap&      bitmap;
            // angle
            Point2F             direction;
            // ctor
            ~DirectionalBlurStatus() noexcept { bitmap.Release(); }
            // default ctor
            inline DirectionalBlurStatus(Asset::Bitmap&& b) : bitmap(b) { }
            // default value
            inline DirectionalBlurStatus(Asset::Bitmap&& b, Default v) :
                EffectStatus(v), bitmap(b) {
                direction = { 4.f, 4.f };
            }
        };
        // directional blur effect
        class DirectionalBlur final : public Drawable::Effect {
            // super class
            using Super = Drawable::Effect;
            // helper
            friend EffectHelper<DirectionalBlur>;
            // dispose
            void dispose() noexcept override;
        public:
            // create this
            static auto Create(const DirectionalBlurStatus&) 
                noexcept->DirectionalBlur*;
            // create this
            static auto CreateSP(const DirectionalBlurStatus& ts) noexcept {
                return (RubyGM::RefPtr<Drawable::DirectionalBlur>(
                    (DirectionalBlur::Create(ts)))
                );
            }
        public:
            // set StandardDeviation
            auto SetStandardDeviation(float sd) noexcept ->Result;
            // get StandardDeviation
            auto GetStandardDeviation() const noexcept { return m_fStdDvi; }
        protected:
            // ctor
            DirectionalBlur(const DirectionalBlurStatus&) noexcept;
            // ctor
            DirectionalBlur(const DirectionalBlur&) = delete;
            // DirectionalBlur
            ~DirectionalBlur() noexcept;
        protected:
            // init
            void init() noexcept;
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
        protected:
            // bitmap asset
            RefPtr<Asset::Bitmap>       m_spAsBitmap;
        };*/
    }
}