#include <bridge/rgmluiConfig.h>
#include <bridge/rgmluiGame.h>


/// <summary>
/// Initializes a new instance of the <see cref="Configure"/> class.
/// </summary>
RubyGM::Bridge::Configure::Configure()noexcept:Super(UIManager, "rubygm.log") {

}


/// <summary>
/// Gets the configure flag.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::Configure::GetConfigureFlag() noexcept->ConfigureFlag {
    return Flag_OutputDebugString | Flag_RenderInAnytime;
}

/// <summary>
/// Gets the name of the locale.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
void RubyGM::Bridge::Configure::GetLocaleName(wchar_t name[]) noexcept {
    std::wcscpy(name, L"en-us");
}

/// <summary>
/// Registers some.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::Configure::RegisterSome() noexcept {
    m_manager.RegisterControlClass(UIGame::CreateControl, "Game");
}


/// <summary>
/// Chooses the adapter.
/// </summary>
/// <param name="adapters">The adapters.</param>
/// <param name="length">The length.</param>
/// <returns></returns>
auto RubyGM::Bridge::Configure::ChooseAdapter(
    const DXGI_ADAPTER_DESC1 adapters[], 
    const size_t length) noexcept -> size_t {
    for (size_t i = 0; i < length; ++i) {
        auto& desc = adapters[i];
        if (!std::wcsncmp(L"NVIDIA", desc.Description, 6))
            return i;
    }
    for (size_t i = 0; i < length; ++i) {
        auto& desc = adapters[i];
        if (!std::wcsncmp(L"Intel", desc.Description, 5))
            return i;
    }
    return length;
}