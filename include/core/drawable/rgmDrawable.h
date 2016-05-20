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
#include "../Util/rgmStruct.h"
#include "../../game/rgmResource.h"

// rubygm namespace
namespace RubyGM { 
    // render context
    struct IGMRenderContext;
    // Drawable namespace
    namespace Drawable {
        // Default
        struct Default {};
        // status for base
        struct BaseStatus {
            // default ctor
            inline BaseStatus() {};
            // default value
            inline BaseStatus(Default) {}
        };
    }
    // Drawable namespace
    namespace Drawable {
        // drawable bitmap class 
        class Bitmap;
        // drawable object
        class RUBYGM_NOVTABLE Object : public Base::Resource {
            // super class
            using Super = Base::Resource;
        public:
            // render object
            virtual void Render(IGMRenderContext&) const noexcept = 0;
            // rasterization, to Drawable::Bitmap, bs = basic_size
            auto Rasterization(SizeF sf, SizeF bs) noexcept->Drawable::Bitmap*;
            // rasterization, to Drawable::Bitmap, sf = scale_factor
            auto Rasterization(float sf, SizeF basic_size) noexcept {
                return this->Rasterization({ sf, sf }, basic_size);
            }
            // rasterization, to Drawable::Bitmap smart ptr
            template<typename ...Args>
            auto RasterizationSP(Args&&... args) noexcept {
                return std::move(RubyGM::CGMPtrA<Drawable::Bitmap>(
                    std::move(this->Rasterization(std::forward<Args>(args)...))
                    ));
            }
        protected:
            // recreate resource
            inline virtual auto recreate() noexcept -> Result override {
                return Result(0);
            }
        protected:
            // ctor
            Object(const BaseStatus&) noexcept {}
            // dtor
            ~Object() noexcept {}
        protected:
        };
    }
}