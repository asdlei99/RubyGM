#define _WIN32_WINNT 0x0A000001
#pragma warning(disable: 4290)
#pragma warning(disable: 4200)
#include <edit/control/rgmMapEditor.h>
#include <core/luiManager.h>


/// <summary>
/// render-chain: render foreground
/// </summary>
/// <returns></returns>
void RubyGM::Control::UIMapEditor::render_chain_foreground() const noexcept {
    // 渲染父类前景
    Super::render_chain_foreground();
}

/// <summary>
/// Renders this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Control::UIMapEditor::Render() const noexcept {
    // 背景渲染
    this->render_chain_background();
    // 主景渲染
    this->render_chain_main();
    // 前景渲染
    this->render_chain_foreground();
}

/// <summary>
/// Finalizes an instance of the <see cref="UIMapEditor"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Control::UIMapEditor::~UIMapEditor() noexcept {

}

/// <summary>
/// Cleanups this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Control::UIMapEditor::cleanup() noexcept {
    // 删前调用
    this->before_deleted();
    // 释放空间
    delete this;
}

/// <summary>
/// Initializes the specified node.
/// </summary>
/// <param name="node">The node.</param>
/// <returns></returns>
void RubyGM::Control::UIMapEditor::initialize(pugi::xml_node node) noexcept {
    // 链式初始化
    Super::initialize(node);

}

/// <summary>
/// Initializes a new instance of the <see cref="UIMapEditor"/> class.
/// </summary>
/// <param name="cp">The cp.</param>
RubyGM::Control::UIMapEditor::UIMapEditor(UIContainer * cp) noexcept :
Super(cp) {

}

/// <summary>
/// Creates the control.
/// </summary>
/// <param name="">The .</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Control::UIMapEditor::CreateControl(
    CreateEventType type,
    pugi::xml_node node) noexcept -> UIControl* {
    UIMapEditor* pControl = nullptr;
    switch (type)
    {
    case LongUI::Type_Initialize:
        break;
    case LongUI::Type_Recreate:
        break;
    case LongUI::Type_Uninitialize:
        break;
    case_LongUI__Type_CreateControl:
        LongUI__CreateWidthCET(UIMapEditor, pControl, type, node);
    }
    return pControl;
}


/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Control::UIMapEditor::Update() noexcept {

    // 父类刷新
    Super::Update();
}

/// <summary>
/// Does the event.
/// </summary>
/// <param name="arg">The argument.</param>
/// <returns></returns>
bool RubyGM::Control::UIMapEditor::DoEvent(
    const LongUI::EventArgument& arg) noexcept {
    // 父类处理
    return Super::DoEvent(arg);
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Control::UIMapEditor::Recreate() noexcept ->HRESULT {
    auto hr = S_OK;
    // 重建父类
    if (SUCCEEDED(hr)) {
        hr = Super::Recreate();
    }
    // 重建本类
    if (SUCCEEDED(hr)) {

    }
    // 返回重建结果
    return hr;
}

#ifdef LongUIDebugEvent

// 重载?特例化 GetIID
template<>
auto LongUI::GetIID<RubyGM::Control::UIMapEditor>() noexcept ->const IID&{
    // {D653BA10-85F5-4B63-ADCA-1773094C8B40}
    static const GUID IID_RubyGM_Control_UIMapEditor = {
        0xd653ba10, 0x85f5, 0x4b63,
        { 0xad, 0xca, 0x17, 0x73, 0x9, 0x4c, 0x8b, 0x40 }
    };
    return IID_RubyGM_Control_UIMapEditor;
}

// UI地图编辑器: 调试信息
bool RubyGM::Control::UIMapEditor::debug_do_event(
    const LongUI::DebugEventInformation& info) const noexcept {
    switch (info.infomation)
    {
    case LongUI::DebugInformation::Information_GetClassName:
        info.str = L"UIMapEditor";
        return true;
    case LongUI::DebugInformation::Information_GetFullClassName:
        info.str = L"::RubyGM::Control::UIMapEditor";
        return true;
    case LongUI::DebugInformation::Information_CanbeCasted:
        // 类型转换
        return *info.iid == LongUI::GetIID<UIMapEditor>()
            || Super::debug_do_event(info);
    default:
        break;
    }
    return false;
}
#endif
