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

#include "rgmGeometry.h"
#include "../Util/rgmUtil.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // Drawable namespace
    namespace Drawable {
        // status for polygon
        struct PolygonStatus : GeometryStatus {
            // points array, at least three sides
            Point2F*            points;
            // length of array, at least three sides
            uint32_t            count;
            // fill mode/rule
            FillRule            fill_rule;
            // default value
            inline PolygonStatus() : GeometryStatus() {
                points = nullptr; count = 0;
                fill_rule = Rule_Evenodd;
            }
        };
        // polygon
        class Polygon : public Drawable::Geometry {
            // super class
            using Super = Drawable::Geometry;
            // dispose object
            void dispose() noexcept override;
        protected:
            // ctor
            Polygon(const PolygonStatus&) noexcept;
            // ctor
            ~Polygon() noexcept;
            // recreate
            //auto recreate() noexcept -> Result override;
        public:
            // create this
            static auto Create(const PolygonStatus&) noexcept ->Polygon*;
            // create this
            static auto CreateSP(const PolygonStatus& ls) noexcept {
                return std::move(RubyGM::RefPtr<Drawable::Polygon>(
                    std::move(Polygon::Create(ls)))
                );
            }
            // render object
            //void Render(IGMRenderContext& rc) const noexcept override;
        public:
        };
    }
}