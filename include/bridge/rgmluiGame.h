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

#include <LongUI.h>
#include <Control/UIControl.h>
#include "../core/graphics/rgmDrawable.h"
#include "../core/graphics/rgmSprite.h"

// rubygm namespace
namespace RubyGM {
    // bridge namespace
    namespace Bridge {
        // game control
        class UIGame final : public LongUI::UIControl {
            // super class
            using Super = LongUI::UIControl;
            // clean this control
            virtual void cleanup() noexcept override;
        public:
            // Render
            virtual void Render() const noexcept override;
            // udate
            virtual void Update() noexcept override;
            // do event
            //virtual bool DoEvent(const LongUI::EventArgument&) noexcept override;
            // do mouse event
            //virtual bool DoMouseEvent(const LongUI::MouseEventArgument&) noexcept override;
            // recreate
            virtual auto Recreate() noexcept ->HRESULT override;
        public:
            // add sprite for game
            auto AddSprite() noexcept->CGMSprite*;
            // get common brush for game
            auto GetCommonBrush() noexcept { return LongUI::SafeAcquire(m_pCommonBrush); }
            // get drawable head for game
            auto GetDrawableHead() const { return this->get_drawable<Drawable::Base, 0>(); }
            // get drawable tail for game
            auto GetDrawableTail() const { return this->get_drawable<Drawable::Base, 1>(); }
        public:
            // ctor
            UIGame(LongUI::UIContainer* cp) noexcept;
            // ctor
            ~UIGame() noexcept;
            // set
            static auto& GetInstance() noexcept { assert(s_pInstance); return *s_pInstance; }
            // create control event
            static auto CreateControl(LongUI::CreateEventType type, pugi::xml_node node) noexcept->LongUI::UIControl*;
        protected:
            // release gpu data
            void release_gpu_data() noexcept;
            // something must do before deleted
            void before_deleted() noexcept { Super::before_deleted(); }
            // initialize, maybe you want call v-method
            void initialize(pugi::xml_node node) noexcept;
            // get drawable
            template<typename T, size_t U> 
            auto get_drawable() const noexcept { return (T*)(m_bufDrawaleHT + sizeof(T) * U); }
        protected:
            // main fiber
            void*                       m_pMainFiber = nullptr;
            // sub fiber
            void*                       m_pSubFiber = nullptr;
        protected:
            // common brush
            ID2D1SolidColorBrush*       m_pCommonBrush = nullptr;
            // window width
            uint32_t                    m_uWndWidth = 0;
            // window height
            uint32_t                    m_uWndHeight = 0;
            // last frame time count
            uint32_t                    m_uLastFrameTime;
            // now fps
            float                       m_fps = 0.f;
            // unused
            uint32_t                    unused = 0;
        public:
            // error code
            uint32_t                    error_code = 0;
        protected:
            // root sprite
            CGMSprite                   m_sprRoot;
            // buffer for drawable head and tail
            char                        m_bufDrawaleHT[sizeof(Drawable::Base) * 2];
#ifdef LongUIDebugEvent
        protected:
            // debug infomation
            virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
            // single
            static UIGame*              s_pInstance;
        };
    }
}

// longui namespace
namespace LongUI {
#ifdef LongUIDebugEvent
    // overload GetIID
    template<> LongUIInline const IID& GetIID<RubyGM::Bridge::UIGame>() { 
        // {FC002D53-A112-49EA-B635-5EA9FDE3E852}
        static const GUID IID_RubyGM_UIGame = { 
            0xfc002d53, 0xa112, 0x49ea, { 0xb6, 0x35, 0x5e, 0xa9, 0xfd, 0xe3, 0xe8, 0x52 }
        };
        return IID_RubyGM_UIGame;
    }
#endif
}