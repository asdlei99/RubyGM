#define _WIN32_WINNT 0x0A000001
#include <core/rubygm.h>
#include <core/drawable/rgmDrawable.h>
#include <core/graphics/rgmGraphics.h>
#include <core/drawable/rgmTextlayout.h>
#include <core/util/rgmImpl.h>
#include <core/Text/rgmTextDetail.h>
#include <core/asset/rgmAssetFont.h>
#include <game/rgmGame.h>
//#include <bridge/rgmluiBridge.h>

// rubygm::impl 命名空间
namespace RubyGM { namespace impl {
    /// <summary>
    /// Creates the typography.
    /// </summary>
    /// <param name="typography">The typography</param>
    /// <returns></returns>
    auto create_typography(IDWriteTypography** typography) noexcept->Result;
    /// <summary>
    /// White spaces?
    /// </summary>
    /// <param name="c">The c.</param>
    /// <returns></returns>
    template<typename T> inline auto white_space(T c) noexcept {
        return ((c) == ' ' || (c) == '\t'); 
    }
}}



// ============================================================================
// ============================== Textlayout ==================================
// ============================================================================

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::recreate() noexcept -> Result {
    // 文本布局是设备无关资源, 无需重新创建
    return Result(S_OK);
}

/// <summary>
/// Creates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::Create(
    const TextlayoutStatus& ts) noexcept -> Textlayout* {
    Textlayout* tmp = nullptr; auto ctx = tmp->get_context();
    const size_t this_len = reinterpret_cast<size_t>(ctx);
    assert(this_len == sizeof(Textlayout));
    // 获取渲染器
    auto renderer = UIManager.GetTextRenderer(ts.renderer);
    // 不可能出错，否则进不到这一步
    assert(renderer);
    // 获取上下文长度
    auto len = renderer->GetContextSizeInByte() + this_len;
    // 释放掉
    renderer->Release();
    // 申请空间
    auto ptr = RubyGM::NormalAlloc(len);
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
    RubyGM::NormalFree(this);
}

// XXX: 移除
namespace RubyGM { namespace Asset {
    // create text format
    auto CreateTextFormat(const wchar_t* name, float size, IDWriteTextFormat** ptr) noexcept->HRESULT;
    // create text layout
    auto CreateTextLayout(const wchar_t* n, uint32_t l,IDWriteTextFormat* f, IDWriteTextLayout** ptr)  noexcept->HRESULT;
}}

/// <summary>
/// Prevents a default instance of the <see cref="Textlayout"/> class from being created.
/// </summary>
RubyGM::Drawable::Textlayout::Textlayout(
    const TextlayoutStatus& ts) noexcept : Super(ts), 
m_pTextRenderer(impl::rubygm(UIManager.GetTextRenderer(ts.renderer))),
basic_color(ts.color) {
    // 获取文本格式接口
    decltype(ts.font->GetFont()) format;
    // 指定字体
    if (ts.font) {
        format = ts.font->GetFont();
    }
    // 默认字体
    else {
        auto deft = Game::GetFontAsset(0);
        format = deft->GetFont();
    }
    // 失败
    if (!format) return;
    // Xml 富文本布局?
    if (ts.isxml) {
        // 格式化文本
        LongUI::DX::FormatTextConfig cfg;
        cfg.format = format; cfg.progress = 1.f;
        cfg.width = ts.width; cfg.height = ts.height;
        cfg.rich_type = LongUI::RichType::Type_Xml; 
        cfg.text_length = 0;
        auto& layout = m_pTextlayout;
        auto len = ts.textlen + 1;
        // 进行格式化
        LongUI::SafeBuffer<wchar_t, 2048>(len, [&](wchar_t* buf) noexcept {
            std::wcsncpy(buf, ts.text, ts.textlen); buf[ts.textlen] = 0;
            if (LongUI::DX::CheckXmlValidity(buf)) {
                auto lay = LongUI::DX::FormatTextXML(cfg, buf);
                layout = static_cast<IGMTextlayout*>(lay);
            }
            else {
                assert(!"BAD XML");
            }
        });
        m_uTextLength = cfg.text_length;
    }
    auto hr = S_OK;
    // 创建布局
    if (!m_pTextlayout) {
        hr = UIManager_DWriteFactory->CreateTextLayout(
            ts.text, ts.textlen, format, ts.width, ts.height,
            reinterpret_cast<IDWriteTextLayout**>(&m_pTextlayout)
        );
        m_uTextLength = ts.textlen;
    }
    // 释放
    format->Release();
    // 记录错误代码
    if (!m_pTextlayout) {
        Game::SetLastErrorCode(Result(hr));
    }
    // 创建渲染上下文
    m_pTextRenderer->MakeContextFromString(this->get_context(), ts.context);
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
void RubyGM::Drawable::Textlayout::Render(IGMRenderContext& rc) const noexcept {
    assert(m_pTextRenderer && m_pTextlayout && "bad action");
    // 设置渲染环境
    m_pTextRenderer->target = &rc;
    m_pTextRenderer->basic_color.color = impl::d2d(this->basic_color);
    // 刻画文本
    IDWriteTextRenderer1* pTextRenderer = m_pTextRenderer;
    m_pTextlayout->Draw(
        this->get_context(),
        pTextRenderer,
        0.f, 0.f
    );
    // 收尾
    m_pTextRenderer->target = nullptr;
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
//auto RubyGM::Drawable::Textlayout::Recreate() noexcept ->Result {
//    return Super::Recreate();
//}

/// <summary>
/// Sets the width of the layout.
/// </summary>
/// <param name="width">The width.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetLayoutWidth(
    float width) noexcept ->Result{
    auto hr = m_pTextlayout->SetMaxWidth(width);
    return Result(hr);
}

/// <summary>
/// Sets the height of the layout.
/// </summary>
/// <param name="height">The height.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetLayoutHeight(
    float height) noexcept ->Result {
    auto hr = m_pTextlayout->SetMaxHeight(height);
    return Result(hr);
}

/// <summary>
/// Gets the metrics for text.
/// </summary>
/// <param name="">The .</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::GetMetrics(
    RubyGM::TextMetrics& metrics) const noexcept ->Result {
    auto hr = m_pTextlayout->GetMetrics(&impl::d2d(metrics));
    return Result(hr);
}

/// <summary>
/// Gets the size from metrics.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::GetSizeFromMetrics(
) const noexcept -> SizeF {
    TextMetrics tm; auto hr = this->GetMetrics(tm);
    // 成功 -> 实际大小
    if (SUCCEEDED(hr)) {
        return{ tm.width, tm.height };
    }
    // 失败 -> 布局大小
    else {
        return{ this->m_fWidth, m_fHeight };
    }
}

/// <summary>
/// Gets the line count.
/// </summary>
/// <returns>count of line</returns>
auto RubyGM::Drawable::Textlayout::GetLineCount(
) const noexcept ->uint32_t {
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
    uint32_t buflen, LineMetrics* buf) const noexcept ->Result {
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
    VAlignment va) noexcept ->Result {
    auto hr = m_pTextlayout->SetParagraphAlignment(impl::d2d(va));
    return Result(hr);
}

/// <summary>
/// Sets the h alignment.
/// </summary>
/// <param name="ha">The ha.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetHAlignment(
    HAlignment ha) noexcept ->Result {
    auto hr = m_pTextlayout->SetTextAlignment(impl::d2d(ha));
    return Result(hr);
}

/// <summary>
/// Sets the reading direction.
/// </summary>
/// <param name="reading">The reading.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetReadingDirection(
    ReadDirection reading) noexcept ->Result {
    auto hr = m_pTextlayout->SetReadingDirection(impl::d2d(reading));
    return Result(hr);
}

/// <summary>
/// Sets the flow direction.
/// </summary>
/// <param name="flow">The flow.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFlowDirection(
    FlowDirection flow) noexcept ->Result {
    auto hr = m_pTextlayout->SetFlowDirection(impl::d2d(flow));
    return Result(hr);
}

/// <summary>
/// Sets the word wrapping.
/// </summary>
/// <param name="warp">The warp.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetWordWrapping(
    WordWrapping warp) noexcept ->Result {
    auto hr = m_pTextlayout->SetWordWrapping(impl::d2d(warp));
    return Result(hr);
}

/// <summary>
/// Sets the font weight.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="fw">The fw.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontWeight(
    TextRange range, FontWeight fw) noexcept ->Result {
    auto hr = m_pTextlayout->SetFontWeight(
        impl::d2d(fw), impl::d2d(range)
    );
    return Result(hr);
}

/// <summary>
/// Sets the font style.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="fs">The fs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontStyle(
    TextRange range, FontStyle fs) noexcept ->Result {
    auto hr = m_pTextlayout->SetFontStyle(
        impl::d2d(fs), impl::d2d(range)
    );
    return Result(hr);
}

/// <summary>
/// Sets the font stretch.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="fs">The fs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontStretch(
    TextRange range, FontStretch fs) noexcept ->Result {
    auto hr = m_pTextlayout->SetFontStretch(
        impl::d2d(fs), impl::d2d(range)
    );
    return Result(hr);
}


/// <summary>
/// Sets the size of the font.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="size">The size.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetFontSize(
    TextRange range, float size) noexcept ->Result {
    auto hr = m_pTextlayout->SetFontSize(size, impl::d2d(range));
    return Result(hr);
}

/// <summary>
/// Sets the name of the font.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontName(
    TextRange range, const wchar_t* name) noexcept ->Result {
    auto hr = m_pTextlayout->SetFontFamilyName(name, impl::d2d(range));
    return Result(hr);
}

/// <summary>
/// Sets the underline.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="underline">if set to <c>true</c> [underline].</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetUnderline(
    TextRange range, bool underline) noexcept ->Result {
    auto hr = m_pTextlayout->SetUnderline(underline, impl::d2d(range));
    return Result(hr);
}

/// <summary>
/// Sets the underline.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="underline">if set to <c>true</c> [underline].</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetStrikethrough(
    TextRange range, bool strike) noexcept ->Result {
    auto hr = m_pTextlayout->SetStrikethrough(strike, impl::d2d(range));
    return Result(hr);
}

/// <summary>
/// Sets the font feature.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="feature">The feature.</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::SetFontFeature(
    TextRange range, const char* feature) noexcept ->Result {
    assert(feature && "bad argument");
    if (!feature) return Result(E_INVALIDARG);
    IDWriteTypography* typography = nullptr;
    auto hr = impl::create_typography(&typography);
    if (!typography) return Result(hr);
    // 创建
    union {
        char                    buffer[4];
        DWRITE_FONT_FEATURE_TAG tag;
    };
    // 遍历字符串
    auto itr = feature;
    while (*itr) {
        // 有效
        if (!impl::white_space(*itr)) {
            std::strncpy(buffer, itr, 4);
            itr += 4;
            hr = Result(typography->AddFontFeature(DWRITE_FONT_FEATURE{tag, 1}));
            if (FAILED(hr)) break;
        }
        else {
            itr += 1;
        }
    }
    // 设置
    if (SUCCEEDED(hr)) {
        hr = m_pTextlayout->SetTypography(typography, impl::d2d(range));
    }
    // 释放数据
    typography->Release();
    return Result(hr);
}

/// <summary>
/// Sets the color of the font.
/// </summary>
/// <param name="range">The range.</param>
/// <param name="color">The color.</param>
/// <returns>error code</returns>
auto RubyGM::Drawable::Textlayout::SetFontColor(
    TextRange range, const ColorF& color) noexcept ->Result {
    auto ce = LongUI::CUIColorEffect::Create(impl::d2d(color));
    if (ce) {
        auto hr = m_pTextlayout->SetDrawingEffect(ce, impl::d2d(range));
        ce->Release();
        return Result(hr);
    }
    else {
        return Result(E_OUTOFMEMORY);
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
    HittestMetrics& htm) noexcept ->Result {
    BOOL bool_trailing_and_inside[2];
    auto hr = m_pTextlayout->HitTestPoint(
        pt.x, pt.y,
        bool_trailing_and_inside + 0,
        bool_trailing_and_inside + 1,
        impl::d2d(&htm)
    );
    trailing_and_inside[0] = !!bool_trailing_and_inside[0];
    trailing_and_inside[1] = !!bool_trailing_and_inside[1];
    return Result(hr);
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
    HittestMetrics& htm) noexcept ->Result {
    auto hr = m_pTextlayout->HitTestTextPosition(
        pos, trailing, &pt.x, &pt.y, impl::d2d(&htm)
    );
    return Result(hr);
}


/// <summary>
/// Hitteses the text range get count.
/// </summary>
/// <param name="range">The range.</param>
/// <returns>count of metrice </returns>
auto RubyGM::Drawable::Textlayout::HittesTextRangeGetCount(
    TextRange range) const noexcept ->Result {
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
    HittestMetrics* buf) noexcept ->Result {
    uint32_t count = 0;
    auto hr = m_pTextlayout->HitTestTextRange(
        range.begin, range.length,
        0.f, 0.f,
        impl::d2d(buf), buflen,
        &count
    );
    assert(count == buflen);
    return Result(hr);

}

// ============================================================================
// =============================== Editable ===================================
// ============================================================================

