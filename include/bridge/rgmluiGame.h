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
#include "../core/drawable/rgmDrawable.h"
#include "../core/graphics/rgmSprite.h"
#include "../game/rgmGame.h"

// wic factory
struct IWICImagingFactory2;

// rubygm namespace
namespace RubyGM {
    // resoure index
    enum AssetIndex : size_t { 
        Index_Bitmap = 0,   // bitmap asset
        Index_Font,         // font asset
        Index_Brush,        // brush asset
        Index_Stroke,       // skroke style 
        RESOURCE_COUNT 
    };
    // bridge namespace
    namespace Bridge {
        // game control
        class UIGame final : public LongUI::UIControl {
            // super class
            using Super = LongUI::UIControl;
            // res class
            using Res = Base::Resource;
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
            auto AddSprite(const SprteStatus& ss) noexcept->CGMSprite*;
            // get root transform
            auto&GetRootTransform() const noexcept { return m_sprRoot.GetTransform(); }
            // get bitmap resource by index, but no add-ref
            auto RefBitmapAsset(uint32_t index) const noexcept -> Asset::Bitmap&;
            // get font resource by index, but no add-ref
            auto RefFontAsset(uint32_t index) const noexcept -> Asset::Font&;
            // get brush resource by index, but no add-ref
            auto RefBrushAsset(uint32_t index) const noexcept -> Asset::Brush&;
            // register a new bitmap resource, return index
            auto RegisterBitmapAsset() noexcept -> uint32_t;
            // register a new brush resource, return index
            auto RegisterBrushAsset(const NBrushProperties&) noexcept -> uint32_t;
            // register a new font resource, return index
            auto RegisterFontAsset(const FontProperties&) noexcept -> uint32_t;
            // get common brush for game
            auto GetCommonBrush() noexcept { return LongUI::SafeAcquire(m_pCommonBrush); }
            // get resource head for game
            auto GetResourceHead() const { return this->get_drawable<Res, 0>(); }
            // get resource tail for game
            auto GetResourceTail() const { return this->get_drawable<Res, 1>(); }
            // get wic factory
            auto&RefWicFactory() const noexcept { return *m_pWicFactory; }
        public:
            // ctor
            UIGame(LongUI::UIContainer* cp) noexcept;
            // ctor
            ~UIGame() noexcept;
            // get cahce dir
            auto&GetCacheDir() noexcept { return m_strCacheDir; }
            // set
            static auto& GetInstance() noexcept { assert(s_pInstance); return *s_pInstance; }
            // create control event
            static auto CreateControl(LongUI::CreateEventType type, pugi::xml_node node) noexcept->LongUI::UIControl*;
        protected:
            // init resource
            void init_resource() noexcept;
            // release asset
            void release_asset() noexcept;
            // release device-dependent resource
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
            // wic factory
            IWICImagingFactory2*        m_pWicFactory = nullptr;
        protected:
            // assert pointers array
            Asset::Object**             m_appAssetPtr[RESOURCE_COUNT];
            // count of assert pointer array
            uint16_t                    m_acAssetPtr[(RESOURCE_COUNT + 3) / 4 * 4];
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
        public:
            // time scale
            float                       time_scale = 1.f;
            // error code
            Result                      error_code = 0;
        protected:
#ifdef LongUIDebugEvent
            // debug infomation
            virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
            // register helper
            template<size_t INDEX, typename T>
            auto register_helper(T create) noexcept -> uint32_t {
                assert(size_t(m_acAssetPtr[INDEX]) <= MAX_SOURCE_EACH);
                if (m_acAssetPtr[INDEX] == MAX_SOURCE_EACH) return 0;
                uint32_t index = m_acAssetPtr[INDEX]++;
                m_appAssetPtr[INDEX][index] = create();
                return index;
            }
        protected:
            // root sprite
            CGMSprite                   m_sprRoot;
            // cache directory
            LongUI::CUIString           m_strCacheDir;
            // buffer for drawable head and tail
            char                        m_bufDrawaleHT[sizeof(Res) * 2];
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