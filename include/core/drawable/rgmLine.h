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

#include "rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // Drawable namespace
    namespace Drawable {
        // status for line
        struct LineStatus : BaseStatus {
            // point 0
            RubyGM::Point2F         point0;
            // point 1
            RubyGM::Point2F         point1;
            // stroke width
            float                   stroke_width;
            // default value
            inline LineStatus(Default v) : BaseStatus(v) {
                point0 = { 0.f, 0.f }; point1 = { 0.f, 0.f };
                stroke_width = 1.f;
            }
        };
        // line 
        class Line : public Drawable::Object {
            // super class
            using Super = Drawable::Object;
            // dispose object
            void dispose() noexcept override;
        protected:
            // ctor
            Line(const LineStatus&) noexcept;
            // ctor
            ~Line() noexcept;
        public:
            // create this
            static auto Create(const LineStatus&) noexcept ->Line*;
            // create this
            static auto CreateSP(const LineStatus& ls) noexcept {
                return std::move(RubyGM::CGMPtr<Drawable::Line>(std::move(Line::Create(ls))));
            }
            // render object
            void Render(IGMRednerContext& rc) const noexcept override;
            // recreate
            //auto Recreate() noexcept -> uint32_t override;
        public:
            // point 0
            RubyGM::Point2F         point0;
            // point 1
            RubyGM::Point2F         point1;
            // stroke width
            float                   stroke_width = 1.f;
        private:
        };
    }
}