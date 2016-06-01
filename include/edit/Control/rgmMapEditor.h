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


#include <luibase.h>
#include <luiconf.h>
#include <Control/UIControl.h>
#include <Control/UIText.h>

// rubygm::control namespace
namespace RubyGM { namespace Control {
    // create event
    using LongUI::CreateEventType;
    // UIContainer
    using LongUI::UIContainer;
    // map editor
    class UIMapEditor final : public LongUI::UIControl {
        // super class
        using Super = LongUI::UIControl;
        // clean this
        void cleanup() noexcept override;
    public:
        // Render
        void Render() const noexcept override;
        // update
        void Update() noexcept override;
        // do event 
        bool DoEvent(const LongUI::EventArgument& arg) noexcept override;
        // recreate 
        auto Recreate() noexcept ->HRESULT override;
    protected:
        // something must do before deleted
        void before_deleted() noexcept { Super::before_deleted(); }
        // render chain -> background
        void render_chain_background() const noexcept { return Super::render_chain_background(); }
        // render chain -> mainground
        void render_chain_main() const noexcept { return Super::render_chain_main(); }
        // render chain -> foreground
        void render_chain_foreground() const noexcept;
    public:
        // create 创建
        static auto CreateControl(CreateEventType, pugi::xml_node) noexcept->UIControl*;
        // ctor: cp- parent in contorl-level
        UIMapEditor(UIContainer* cp) noexcept;
    protected:
        // initialize, maybe you want call v-method
        void initialize(pugi::xml_node node) noexcept;
        // dtor
        ~UIMapEditor() noexcept;
        // copy ctor = delete
        UIMapEditor(const UIMapEditor&) = delete;
    protected:

#ifdef LongUIDebugEvent
    protected:
        // debug infomation
        virtual bool debug_do_event(const LongUI::DebugEventInformation&) const noexcept override;
#endif
    };
}}

#ifdef LongUIDebugEvent
namespace LongUI {
    // 重载?特例化 GetIID
    template<> const IID& GetIID<RubyGM::Control::UIMapEditor>() noexcept;
}
#endif