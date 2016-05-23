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
    // ellipse
    struct EllipseF {
        // point of ellipse-center
        Point2F         point;
        // radius
        float           rx, ry;
    };
    // Drawable namespace
    namespace Drawable {
        // always draw circle
        struct AlwaysCircle : Default {};
        // status for rect
        struct EllipseStatus : VectorStatus {
            // ellipse data
            EllipseF        ellipse;
            // ctor
            EllipseStatus() : VectorStatus() {}
            // default value
            inline EllipseStatus(Default v) : VectorStatus(v) {
                ellipse.point = { 8.f, 8.f };
                ellipse.rx = 8.f;
                ellipse.ry = 8.f;
            }
            // Circle, ALWAYS ignore rx
            inline EllipseStatus(AlwaysCircle v) : VectorStatus(v) {
                ellipse.point = { 8.f, 8.f };
                ellipse.rx = 8.f;
                ellipse.ry = -1.f;
            }
        };
        // ellipse 
        class Ellipse : public Drawable::Vector {
            // super class
            using Super = Drawable::Vector;
            // dispose object
            void dispose() noexcept override;
        protected:
            // ctor
            Ellipse(const EllipseStatus&) noexcept;
            // ctor
            ~Ellipse() noexcept;
            // recreate
            //auto recreate() noexcept -> Result override;
        public:
            // create this
            static auto Create(const EllipseStatus&) noexcept ->Ellipse*;
            // create this
            static auto CreateSP(const EllipseStatus& ls) noexcept {
                return std::move(RubyGM::CGMPtrA<Drawable::Ellipse>(
                    std::move(Ellipse::Create(ls)))
                );
            }
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        public:
            // ellipse
            EllipseF                ellipse;
        };
    }
}