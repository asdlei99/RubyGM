#include <bridge/rgmluiGame.h>


/// <summary>
/// Adds the sprite.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::AddSprite() noexcept -> CGMSprite* {
    try {
        auto& sprite = m_sprRoot.AddChild();
        return &sprite;
    }
    catch (...) {
        return nullptr;
    }
}


/// <summary>
/// Adds the new.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::CGMSprite::AddNew(const SprteStatus& status) noexcept -> CGMSprite* {
    UNREFERENCED_PARAMETER(status);
    // TODO: status to sprite
    return RubyGM::Bridge::UIGame::GetInstance().AddSprite();
}

/// <summary>
/// Sets the last error code.
/// </summary>
/// <param name="code">The code.</param>
/// <returns></returns>
void RubyGM::SetLastErrorCode(uint32_t code) noexcept {
    RubyGM::Bridge::UIGame::GetInstance().error_code = code;
}

/// <summary>
/// Gets the last error code.
/// </summary>
/// <returns></returns>
auto RubyGM::GetLastErrorCode() noexcept->uint32_t {
    return RubyGM::Bridge::UIGame::GetInstance().error_code;
}


/// <summary>
/// Uninitializes this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::GetLastDrawableObject() noexcept -> Drawable::Base* {
    return RubyGM::Bridge::UIGame::GetInstance().GetDrawableTail();
}

/// <summary>
/// Gets the color brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::GetColorBrush() noexcept ->IGMBrush* {
    auto brush = RubyGM::Bridge::UIGame::GetInstance().GetCommonBrush();
    return reinterpret_cast<IGMBrush*>(brush);
}


// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // get now time
    auto get_time() noexcept -> uint32_t { return ::timeGetTime(); }
}}
