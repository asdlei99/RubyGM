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
    struct IGMRednerContext;
    // brush
    struct IGMBrush;
    // Drawable namespace
    namespace Drawable {
        // Default
        struct Default {};
        // status for base
        struct BaseStatus {
            // basic color
            RubyGM::ColorF      color;
            // default ctor
            inline BaseStatus() {};
            // default value
            inline BaseStatus(Default) {
                color.b = color.g = color.r = 0.f;
                color.a = 1.f;
            }
        };
        // alloc for drawable object
        auto Alloc(size_t) noexcept->void*;
        // free for drawable object
        void Free(void*) noexcept;
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
    }
    // Drawable namespace
    namespace Drawable {
        // drawable object
        class RUBYGM_NOVTABLE Object : public Base::Resource {
            // super class
            using Super = Base::Resource;
        public:
            // set color
            void SetColor(const RubyGM::ColorF& color) noexcept;
            // set brush asset
            void SetBrush(IGMBrush* brush) noexcept;
        public:
            // recreate resource
            virtual auto Recreate() noexcept -> uint32_t override;
            // render object
            virtual void Render(IGMRednerContext&) const noexcept = 0;
        protected:
            // before render
            void before_render() const noexcept;
            // set brush mode
            void set_brush_mode(BrushMode m) { m_u32Data = uint32_t(m); }
            // get brush mode
            auto get_brush_mode() const { return BrushMode(m_u32Data); }
        protected:
            // ctor
            Object(const BaseStatus&) noexcept;
            // dtor
            ~Object() noexcept;
        protected:
            // brush
            IGMBrush*               m_pBrush;
            // color
            RubyGM::ColorF          m_color;
        };
    }
}