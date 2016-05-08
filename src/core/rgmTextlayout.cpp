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

namespace RubyGM {
    namespace Resource {
        // create text format
        auto CreateTextFormat(const wchar_t* name, float size, IDWriteTextFormat** ptr) noexcept->HRESULT;
        // create text layout
        auto CreateTextLayout(const wchar_t* n, uint32_t l,IDWriteTextFormat* f, IDWriteTextLayout** ptr)  noexcept->HRESULT;
    }
}

/// <summary>
/// Prevents a default instance of the <see cref="Textlayout"/> class from being created.
/// </summary>
RubyGM::Drawable::Textlayout::Textlayout() noexcept {
    IDWriteTextFormat* format = nullptr;
    Resource::CreateTextFormat(L"KaiTi", 32.f, &format);
    if (format) {
        auto t = L"Hello 世界!";
        Resource::CreateTextLayout(t, std::wcslen(t), format, (IDWriteTextLayout**)(&m_pTextlayout));;
    }
    SafeRelease(format);
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
    rc.DrawTextLayout(D2D1::Point2F(), m_pTextlayout, m_pBrush);
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