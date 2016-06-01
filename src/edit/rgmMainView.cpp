#define _WIN32_WINNT 0x0A000001
#pragma warning(disable: 4290)
#pragma warning(disable: 4200)
#include <edit/rgmMainView.h>
#include <game/rgmGame.h>
#include <Core/luiWindow.h>
#include <Core/luiManager.h>


/// <summary>
/// Cleanups this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Editor::MainViewport::cleanup() noexcept {
    // 删前调用
    this->before_deleted();
    // 释放空间
    delete this;
}


/// <summary>
/// Does the LongUI event.
/// </summary>
/// <param name="arg">The argument.</param>
/// <returns></returns>
bool RubyGM::Editor::MainViewport::DoEvent(
    const LongUI::EventArgument& arg) noexcept {
    switch (arg.event)
    {
    case LongUI::Event::Event_TreeBuildingFinished:
        this->tree_created();
        __fallthrough;
    default:
        return Super::DoEvent(arg);
    }
}

/// <summary>
/// called after control created
/// </summary>
/// <returns></returns>
void RubyGM::Editor::MainViewport::tree_created() noexcept {
    auto window = m_pWindow;
    if (const auto control = (window->FindControl("btnRecreate"))) {
        control->Add_OnClicked([](LongUI::UIControl*) {
            UIManager.RecreateResources();
            return true;
        });
    }
    if (const auto control = (window->FindControl("btnFullDraw"))) {
        control->Add_OnClicked([window](LongUI::UIControl*) {
            window->InvalidateWindow();
            return true;
        });
    }
    if (const auto control = (window->FindControl("btnPauseResume"))) {
        const auto slider = window->FindControl("sldTimeScale");
        control->Add_OnClicked([slider](LongUI::UIControl*) {
            float ts = RubyGM::Game::GetTimeScale();
            ts = ts == 0.f ? 1.f : 0.f;
            slider->SetFloat(ts);
            RubyGM::Game::SetTimeScale(ts);
            return true;
        });
    }
    if (const auto control = (window->FindControl("sldTimeScale"))) {
        control->Add_OnChanged([](LongUI::UIControl* sld) {
            RubyGM::Game::SetTimeScale(sld->GetFloat());
            return true;
        });
    }
    window->ShowWindow(SW_SHOWNORMAL);
}