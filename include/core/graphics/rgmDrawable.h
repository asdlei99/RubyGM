#pragma once
/**
* Copyright (c) 2015-2015 dustpg   mailto:dustpg@gmail.com
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

// rubygm namespace
namespace RubyGM { 
    // render context
    struct GMRednerContext;
    // brush
    struct GMBrush;
    // manager
    class CGMManager;
    // Drawable namespace
    namespace Drawable {
        // get color brush
        auto GetColorBrush() noexcept ->GMBrush*;
        // Base class
        class Base;
        // get last Base pointer
        auto GetLastDrawableObject() noexcept ->Base*;
        // render mode
        enum RenderMode : uint32_t {
            Mode_Fast = 0,      // fast
            Mode_SaveMemory,    // save memory
        };
        // bursh mode
        enum BrushMode : uint32_t {
            Mode_Color = 0,     // color brush
            Mode_Other,         // other brush
        };
        // base class
        class Base {
            // friend class
            friend class CGMManager;
        public:
            // recreate all object
            static auto RecreateAll() noexcept ->uint32_t;
        public:
            // add ref-count
            auto AddRef() noexcept ->uint32_t { return ++m_cRef; }
            // release ref-count
            auto Release() noexcept ->uint32_t;
            // set color
            void SetColor(const RubyGM::ColorF& color) noexcept;
            // set brush
            void SetBrush(GMBrush* brush) noexcept;
        public:
            // render object
            virtual void Render(GMRednerContext&) const noexcept = 0;
            // recreate
            virtual auto Recreate() noexcept -> uint32_t;
        protected:
            // dispose object
            virtual void dispose() noexcept = 0;
            // before render
            void before_render() const noexcept;
        protected:
            // ctor
            Base() noexcept;
            // dtor
            ~Base() noexcept;
        protected:
            // ref-count
            uint32_t            m_cRef = 1;
            // bool set
            BrushMode           m_modeBrush = BrushMode::Mode_Color;
            // color
            ColorF              m_color;
            // prve
            Base*               m_pPrve;
            // prve
            Base*               m_pNext;
            // brush
            GMBrush*            m_pBrush = Drawable::GetColorBrush();
        };
    }
}