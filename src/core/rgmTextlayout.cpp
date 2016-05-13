#define _WIN32_WINNT 0x0A000001
#include <core/rubygm.h>
#include <core/drawable/rgmDrawable.h>
#include <core/graphics/rgmGraphics.h>
#include <core/drawable/rgmTextlayout.h>
#include <core/util/rgmImpl.h>
#include <core/Text/rgmTextDetail.h>
#include <core/asset/rgmAssetFont.h>
#include <game/rgmGame.h>


/// <summary>
/// Creates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::Create(const TextStatus& ts) noexcept -> Textlayout* {
    // 获取渲染器
    auto renderer = UIManager.GetTextRenderer(ts.renderer);
    // 不可能出错，否则进不到这一步
    assert(renderer);
    // 获取上下文长度
    auto len = renderer->GetContextSizeInByte() + sizeof(Textlayout);
    // 释放掉
    renderer->Release();
    // 申请空间
    auto ptr = Drawable::Alloc(len);
    // 申请失败
    if (!ptr) return nullptr;
    // 创建对象 不允许创建布局失败
    auto layout = new(ptr) Textlayout(ts);
    // 文本创建失败, 返回空指针
    if (layout && !layout->m_pTextlayout) {
#ifdef _DEBUG
        assert(!"test");
#endif
        layout->dispose();
        layout = nullptr;
    }
    // 正常返回
    return layout;
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Textlayout::dispose() noexcept {
    // 手动调用析构函数
    this->Textlayout::~Textlayout();
    // 释放空间
    Drawable::Free(this);
}

// XXX: 移除
namespace RubyGM {
    namespace Asset {
        // create text format
        auto CreateTextFormat(const wchar_t* name, float size, IDWriteTextFormat** ptr) noexcept->HRESULT;
        // create text layout
        auto CreateTextLayout(const wchar_t* n, uint32_t l,IDWriteTextFormat* f, IDWriteTextLayout** ptr)  noexcept->HRESULT;
    }
}

/// <summary>
/// Prevents a default instance of the <see cref="Textlayout"/> class from being created.
/// </summary>
RubyGM::Drawable::Textlayout::Textlayout(const TextStatus& ts) noexcept : Super(ts), 
m_pTextRenderer(impl::rubygm(UIManager.GetTextRenderer(ts.renderer))) {
    // 创建文本格式/字体
    auto format = ts.font.GetFont(); if (!format) return;
    // 创建布局
    auto hr = UIManager_DWriteFactory->CreateTextLayout(
        ts.text, ts.textlen, format, ts.width, ts.height,
        reinterpret_cast<IDWriteTextLayout**>(&m_pTextlayout)
    );
    // 释放
    format->Release();
    // 记录错误代码
    if (!m_pTextlayout) {
        Game::SetLastErrorCode(uint32_t(hr));
    }
    // 创建渲染上下文
    m_pTextRenderer->MakeContextFromString(m_bufDrawContext, ts.context);
}

/// <summary>
/// Finalizes an instance of the <see cref="Textlayout"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Textlayout::~Textlayout() noexcept {
    RubyGM::SafeRelease(m_pTextRenderer);
    RubyGM::SafeRelease(m_pTextlayout);
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Textlayout::Render(IGMRednerContext& rc) const noexcept {
    assert(m_pTextRenderer && m_pTextlayout && "bad action");
    // 设置渲染环境
    m_pTextRenderer->target = &rc;
    m_pTextRenderer->basic_color.color = impl::d2d(m_color);
    // 刻画文本
    m_pTextlayout->Draw(
        const_cast<size_t*>(m_bufDrawContext),
        m_pTextRenderer,
        0.f, 0.f
    );
    // 收尾
    m_pTextRenderer->target = nullptr;
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::Recreate() noexcept -> uint32_t {
    return uint32_t(S_OK);
}

/// <summary>
/// Sets the width of the layout.
/// </summary>
/// <param name="width">The width.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetLayoutWidth(
    float width) noexcept ->uint32_t{
    auto hr = m_pTextlayout->SetMaxWidth(width);
    return uint32_t(hr);
}

/// <summary>
/// Sets the height of the layout.
/// </summary>
/// <param name="height">The height.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetLayoutHeight(
    float height) noexcept ->uint32_t {
    auto hr = m_pTextlayout->SetMaxHeight(height);
    return uint32_t(hr);
}

/// <summary>
/// Gets the metrics for text.
/// </summary>
/// <param name="">The .</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::GetMetrics(
    RubyGM::TextMetrics& metrics) const noexcept ->uint32_t {
    auto hr = m_pTextlayout->GetMetrics(&impl::d2d(metrics));
    return uint32_t(hr);
}

/// <summary>
/// Gets the line count.
/// </summary>
/// <returns>count of line</returns>
auto RubyGM::Drawable::Textlayout::GetLineCount(
) const noexcept -> uint32_t {
    uint32_t count = 0;
    m_pTextlayout->GetLineMetrics(nullptr, 0, &count);
    return count;
}

/// <summary>
/// Gets the line metrics.
/// </summary>
/// <param name="buflen">The buflen.</param>
/// <param name="buf">The buf.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::GetLineMetrics(
    uint32_t buflen, LineMetrics* buf) const noexcept -> uint32_t {
    uint32_t count = 0;
    auto hr = m_pTextlayout->GetLineMetrics(
        impl::d2d(buf), buflen, &count
    );
    assert(buflen != count);
    return hr;
}

/// <summary>
/// Sets the v alignment.
/// </summary>
/// <param name="va">The va.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetVAlignment(
    VAlignment va) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetParagraphAlignment(impl::d2d(va));
    return uint32_t(hr);
}

/// <summary>
/// Sets the h alignment.
/// </summary>
/// <param name="ha">The ha.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetHAlignment(
    HAlignment ha) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetTextAlignment(impl::d2d(ha));
    return uint32_t(hr);
}

/// <summary>
/// Sets the reading direction.
/// </summary>
/// <param name="reading">The reading.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetReadingDirection(
    ReadDirection reading) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetReadingDirection(impl::d2d(reading));
    return uint32_t(hr);
}

/// <summary>
/// Sets the flow direction.
/// </summary>
/// <param name="flow">The flow.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFlowDirection(
    FlowDirection flow) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetFlowDirection(impl::d2d(flow));
    return uint32_t(hr);
}

/// <summary>
/// Sets the word wrapping.
/// </summary>
/// <param name="warp">The warp.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetWordWrapping(
    WordWrapping warp) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetWordWrapping(impl::d2d(warp));
    return uint32_t(hr);
}

/// <summary>
/// Sets the font weight.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="fw">The fw.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontWeight(
    TextRange range, FontWeight fw) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetFontWeight(
        impl::d2d(fw), impl::d2d(range)
    );
    return uint32_t(hr);
}

/// <summary>
/// Sets the font style.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="fs">The fs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontStyle(
    TextRange range, FontStyle fs) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetFontStyle(
        impl::d2d(fs), impl::d2d(range)
    );
    return uint32_t(hr);
}

/// <summary>
/// Sets the font stretch.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="fs">The fs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontStretch(
    TextRange range, FontStretch fs) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetFontStretch(
        impl::d2d(fs), impl::d2d(range)
    );
    return uint32_t(hr);
}


/// <summary>
/// Sets the size of the font.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="size">The size.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetFontSize(
    TextRange range, float size) noexcept ->uint32_t {
    auto hr = m_pTextlayout->SetFontSize(size, impl::d2d(range));
    return uint32_t(hr);
}

/// <summary>
/// Sets the name of the font.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontName(
    TextRange range, const wchar_t* name) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetFontFamilyName(name, impl::d2d(range));
    return uint32_t(hr);
}

/// <summary>
/// Sets the underline.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="underline">if set to <c>true</c> [underline].</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetUnderline(
    TextRange range, bool underline) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetUnderline(underline, impl::d2d(range));
    return uint32_t(hr);
}

/// <summary>
/// Sets the underline.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="underline">if set to <c>true</c> [underline].</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetStrikethrough(
    TextRange range, bool strike) noexcept -> uint32_t {
    auto hr = m_pTextlayout->SetStrikethrough(strike, impl::d2d(range));
    return uint32_t(hr);
}

/// <summary>
/// Sets the color of the font.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="color">The color.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetFontColor(
    TextRange range, const ColorF& color) noexcept -> uint32_t {
    auto ce = LongUI::CUIColorEffect::Create(impl::d2d(color));
    if (ce) {
        auto hr = m_pTextlayout->SetDrawingEffect(ce, impl::d2d(range));
        ce->Release();
        return uint32_t(hr);
    }
    else {
        return uint32_t(E_OUTOFMEMORY);
    }
}

/// <summary>
/// Hittests the point.
/// </summary>
/// <param name="pt">The pt.</param>
/// <param name="trailing_and_inside">The trailing_and_inside.</param>
/// <param name="htm">The HTM.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::HittestPoint(
    Point2F pt, bool trailing_and_inside[2], 
    HittestMetrics& htm) noexcept -> uint32_t {
    BOOL bool_trailing_and_inside[2];
    auto hr = m_pTextlayout->HitTestPoint(
        pt.x, pt.y,
        bool_trailing_and_inside + 0,
        bool_trailing_and_inside + 1,
        impl::d2d(&htm)
    );
    trailing_and_inside[0] = !!bool_trailing_and_inside[0];
    trailing_and_inside[1] = !!bool_trailing_and_inside[1];
    return uint32_t(hr);
}


/// <summary>
/// Hitteses the text position.
/// </summary>
/// <param name="pos">The position.</param>
/// <param name="trailing">if set to <c>true</c> [trailing].</param>
/// <param name="pt">The pt.</param>
/// <param name="htm">The HTM.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::HittesTextPos(
    uint32_t pos, bool trailing, Point2F& pt, 
    HittestMetrics& htm) noexcept -> uint32_t {
    auto hr = m_pTextlayout->HitTestTextPosition(
        pos, trailing, &pt.x, &pt.y, impl::d2d(&htm)
    );
    return uint32_t(hr);
}


/// <summary>
/// Hitteses the text range get count.
/// </summary>
/// <param name="range">The range.</param>
/// <returns>count of metrice </returns>
auto RubyGM::Drawable::Textlayout::HittesTextRangeGetCount(
    TextRange range) const noexcept -> uint32_t {
    uint32_t count = 0;
    m_pTextlayout->HitTestTextRange(
        range.begin, range.length,
        0.f, 0.f,
        nullptr, 0,
        &count
    );
    return count;
}


/// <summary>
/// Hitteses the text range.
/// </summary>
/// <param name="">The .</param>
/// <param name="buflen">The buflen.</param>
/// <param name="buf">The buf.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::HittesTextRange(
    TextRange range, uint32_t buflen, 
    HittestMetrics* buf) noexcept -> uint32_t {
    uint32_t count = 0;
    auto hr = m_pTextlayout->HitTestTextRange(
        range.begin, range.length,
        0.f, 0.f,
        impl::d2d(buf), buflen,
        &count
    );
    assert(count == buflen);
    return uint32_t(hr);

}