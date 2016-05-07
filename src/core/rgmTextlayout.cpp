#include <core/rubygm.h>
#include <core/graphics/rgmDrawable.h>
#include <core/graphics/rgmGraphics.h>
#include <core/graphics/Drawable/rgmTextlayout.h>
#include <core/util/rgmImpl.h>
#include <core/Text/rgmTextDetail.h>


/// <summary>
/// Creates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::Create() noexcept -> Textlayout* {
    // 只读文本 不允许创建布局失败
    auto layout = new(std::nothrow) Textlayout;
    // 文本创建失败
    if (layout && !layout->m_pTextlayout) {
#ifdef _DEBUG
        assert(!"test");
#endif
        layout->dispose();
        layout = nullptr;
    }
    return layout;
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Textlayout::dispose() noexcept {
    delete this;
}


/// <summary>
/// Prevents a default instance of the <see cref="Textlayout"/> class from being created.
/// </summary>
RubyGM::Drawable::Textlayout::Textlayout() noexcept {
}

/// <summary>
/// Finalizes an instance of the <see cref="Textlayout"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Textlayout::~Textlayout() noexcept {
    RubyGM::SafeRelease(m_pTextlayout);
}


/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Textlayout::Render(IGMRednerContext& rc) const noexcept {
    assert(m_pTextlayout && "bad action");
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::Recreate() noexcept -> uint32_t {
    return uint32_t(S_OK);
}

/// <summary>
/// Sets the size of the layout.
/// </summary>
/// <param name="w">The w.</param>
/// <param name="h">The h.</param>
/// <returns></returns>
void RubyGM::Drawable::Textlayout::SetLayoutSize(float w, float h) noexcept {
    m_pTextlayout->SetMaxWidth(w);
    m_pTextlayout->SetMaxHeight(h);
}

/// <summary>
/// Gets the metrics for text.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::Drawable::Textlayout::GetMetrics(TextMetrics& metrics) const noexcept {
    m_pTextlayout->GetMetrics(&impl::d2d(metrics));
}