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
#include "rgmAsset.h"
#include "../../game/rgmResource.h"
#include "../util/rgmStrokeStruct.h"

// rubygm namespace
namespace RubyGM {
    // stroke style
    struct IGMStrokeStyle;
    // asset namespace
    namespace Asset {
        // stroke style resource
        class Stroke final : public RubyGM::Asset::Object {
            // super class
            using Super = Asset::Object;
        public:
            // create font with FontProperties
            static auto Create(const StrokeStyle&) noexcept ->Stroke&;
            // get Stroke style graphics interface
            auto GetStroke() noexcept -> IGMStrokeStyle*;
        protected:
            // ctor
            Stroke(const StrokeStyle&) noexcept;
            // ctor
            Stroke(const Stroke&) noexcept = delete;
            // ctor
            ~Stroke() noexcept;
        private:
            // dispose object
            void dispose() noexcept override;
        public:
            // set to Low Occupancy to save memory
            void LowOccupancy() noexcept override;
        protected:
            // stroke style graphics interface
            IGMStrokeStyle*         m_pStrokeStyle = nullptr;
            // style
            BaseStrokeStyle         m_style;
            // dash array
            float                   m_dashes[0];
        };
    }
}