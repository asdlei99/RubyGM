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
#include "../rgmDrawable.h"
#include "../../Util/rgmUtil.h"
#include "../../asset/rgmAsset.h"
#include <utility>


// guid
struct _GUID;

// rubygm namespace
namespace RubyGM {
    // effect graphics interface
    struct IGMEffect;
    // effect output graphics interface
    struct IGMEffectOutput;
    // resource namespace
    namespace Drawable {
        // status for bitmap
        struct EffectStatus : BaseStatus {
            // src rect
            RectF           src_rect;
            // des rect
            RectF           des_rect;
            // default ctor
            inline EffectStatus()  { }
            // default value
            inline EffectStatus(Default v) : BaseStatus(v) {
                src_rect = { 0.f };
                des_rect = { 0.f };
            }
        };
        // effect implement helper
        template<class T> struct EffectHelper {};
        // effect
        class Effect : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        protected:
            // ctor
            Effect(const EffectStatus&, const _GUID& iid) noexcept;
            // ctor
            Effect(const Effect&) = delete;
            // dtor
            ~Effect() noexcept;
            // recreate resource
            virtual auto recreate() noexcept -> Result override;
            // effect is ok?
            bool is_ok() const noexcept { return !!m_pEffect; }
        protected:
            // guid of effect
            const _GUID&            m_refGuid;
            // effect gi
            IGMEffect*              m_pEffect = nullptr;
            // effect output gi
            IGMEffectOutput*        m_pOutput = nullptr;
        public:
            // source rect
            RectF                   src_rect{ 0.f};
            // des rect
            RectF                   des_rect{ 0.f};
        protected:
            // single input init
            template<typename T> 
            inline void single_input_init(T* obj) {
                m_pEffect->SetInput(0, obj);
                if (this->src_rect.right == 0.f) {
                    auto size = obj->GetSize();
                    this->src_rect.right = size.width;
                    this->src_rect.bottom = size.height;
                }
                if (this->des_rect.right == 0.f) {
                    this->des_rect.right = this->src_rect.right;
                    this->des_rect.bottom = this->src_rect.bottom;
                }
            }
        };
    }
}