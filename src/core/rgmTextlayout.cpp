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

auto RubyGM::Drawable::Textlayout::SetLayoutWidth(
    float width) noexcept ->uint32_t{
    auto hr = m_pTextlayout->SetMaxWidth(width);
    return uint32_t(hr);
}

auto RubyGM::Drawable::Textlayout::SetLayoutHeight(
    float height) noexcept ->uint32_t {
    auto hr = m_pTextlayout->SetMaxHeight(height);
    return uint32_t(hr);
}

/// <summary>
/// Gets the metrics for text.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Drawable::Textlayout::GetMetrics(
    RubyGM::TextMetrics& metrics) const noexcept ->uint32_t {
    auto hr = m_pTextlayout->GetMetrics(&impl::d2d(metrics));
    return uint32_t(hr);
}