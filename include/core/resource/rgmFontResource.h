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

#include "rgmResource.h"
#include "../text/rgmTextStruct.h"

#pragma warning(disable: 4200)

// rubygm namespace
namespace RubyGM {
    // resource namespace
    namespace Resource {
        // base class
        class Font final : public Base {
        public:
            // create font with FontProperties
            static auto Create(const FontProperties&) noexcept ->Font&;
        public:
            // dispose
            virtual void Dispose() noexcept override;
            // recreate
            virtual auto Recreate() noexcept ->uint32_t override;
            // set to Low Occupancy to save memory
            virtual void LowOccupancy() noexcept override;
            // get font
            auto GetFont() noexcept ->IGMFont*;
            // ctor
            Font(const FontProperties& prop, size_t len) noexcept;
            // ctor
            ~Font() noexcept;
        private:
            // get font prop
            auto& get_prop() { return *reinterpret_cast<FontProperties*>(m_bufFontProp); }
        private:
            // font
            IGMFont*        m_pTextFormat = nullptr;
            // buffer for font properties
            char            m_bufFontProp[sizeof(FontProperties)];
            // buffer for font name
            wchar_t         m_bufFontName[0];
        };
    }
}
