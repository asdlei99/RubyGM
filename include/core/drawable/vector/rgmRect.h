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

#include "rgmVector.h"
#include "../Util/rgmUtil.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // rounded rect
    struct RoundedRectF {
        // rect
        RectF           rect;
        // radius
        float           rx, ry;
    };
    // Drawable namespace
    namespace Drawable {
        // always right angle
        struct AlwaysRightAngle {};
        // status for rect
        struct RectStatus : VectorStatus {
            // Rounded Rect data
            RoundedRectF    rd_rect;
            // default value
            inline RectStatus() : VectorStatus() {
                rd_rect.rect = { 0.f, 0.f, 1.f , 1.f };
                rd_rect.rx = 0.f;
                rd_rect.ry = 0.f;
            }
            // Right Angle, ALWAYS ignore rx/ry
            inline RectStatus(AlwaysRightAngle ) : VectorStatus() {
                rd_rect.rect = { 0.f, 0.f, 1.f , 1.f };
                rd_rect.rx = -1.f;
                rd_rect.ry = -1.f;
            }
        };
        // line 
        class Rect : public Drawable::Vector {
            // super class
            using Super = Drawable::Vector;
            // dispose object
            void dispose() noexcept override;
        protected:
            // ctor
            Rect(const RectStatus&) noexcept;
            // ctor
            ~Rect() noexcept;
            // recreate
            //auto recreate() noexcept -> Result override;
        public:
            // create this
            static auto Create(const RectStatus&) noexcept ->Rect*;
            // create this
            static auto CreateSP(const RectStatus& ls) noexcept {
                return std::move(RubyGM::RefPtr<Drawable::Rect>(
                    std::move(Rect::Create(ls)))
                );
            }
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        public:
            // rect
            RoundedRectF            rd_rect;
        };
    }
}