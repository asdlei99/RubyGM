#include <core/asset/rgmAssetStroke.h>
#include <core/graphics/rgmGraphics.h>
#include <core/Drawable/rgmVector.h>
#include <core/util/rgmImpl.h>
#include <core/rubygm.h>
#include <game/rgmGame.h>
#include <bridge/rgmluiBridge.h>


/// <summary>
/// Initializes a new instance of the <see cref="Vector"/> class.
/// </summary>
/// <param name="vs">The vs.</param>
RubyGM::Drawable::Vector::Vector(const VectorStatus& vs) noexcept:
Super(vs),
stroke_color(vs.stroke_color),
fill_color(vs.stroke_color),
m_pAsStrokeStyle(vs.stroke_style),
m_pAsBrushStroke(vs.stroke_brush),
m_pAsBrushFill(vs.fill_brush),
stroke_width(vs.stroke_width),
m_bUseStrokeColor(!vs.stroke_brush),
m_bUseFillColor(!vs.fill_brush){
    // 重置相关资源图形接口数据
    this->reset_asset();
    // 使用特定笔刷 -> 设置颜色alpha到1.f
    if (vs.stroke_brush) this->stroke_color.a = 1.f;
    if (vs.fill_brush) this->fill_color.a = 1.f;
    // 调试断点
#ifdef _DEBUG
    int bk; bk = 9;
#endif
}

/// <summary>
/// reset gpu asset for this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Vector::reset_asset() noexcept -> Result {
    // 释放相关资源
    RubyGM::SafeRelease(m_pGiStrokeStyle);
    RubyGM::SafeRelease(m_pGiBrushStroke);
    RubyGM::SafeRelease(m_pGiBrushFill);
    // 获取笔触资源
    m_pGiBrushStroke = m_pAsBrushStroke ?
        m_pAsBrushStroke->GetBrush() : Bridge::GetCommonBrush();
    // 获取填充资源
    m_pGiBrushFill = m_pAsBrushFill ?
        m_pAsBrushFill->GetBrush() : Bridge::GetCommonBrush();
    // 获取笔触风格的图形接口数据 -- 本项允许null
    if (m_pAsStrokeStyle) {
        m_pGiStrokeStyle = m_pAsStrokeStyle->GetStrokeStyle();
    }
    // 返回结果
    return (m_pGiBrushStroke && m_pGiBrushFill) ?
        Result(S_OK) : Result(E_FAIL);
}

/// <summary>
/// Set the stroke brushes, call this before draw stroke
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Vector::set_stroke_brush() const noexcept {
    if (m_bUseStrokeColor) {
        impl::d2d<ID2D1SolidColorBrush>(
            m_pGiBrushStroke, IID_ID2D1SolidColorBrush
            )->SetColor(&impl::d2d(this->stroke_color));
    }
}

/// <summary>
/// Set fill brushes, call this before draw fill
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Vector::set_fill_brush() const noexcept {
    if (m_bUseFillColor) {
        impl::d2d<ID2D1SolidColorBrush>(
            m_pGiBrushFill, IID_ID2D1SolidColorBrush
            )->SetColor(&impl::d2d(this->fill_color));
    }
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Vector::recreate() noexcept -> Result {
    Result hr = Result(S_OK);
    // 安全重建
    auto safe_recreate = [](Asset::Object* obj) noexcept {
        return obj ? obj->Recreate() : Result(S_OK);
    };
    // 重建父类
    if (SUCCEEDED(hr)) {
        hr = Super::recreate();
    }
    // 重建资源对象 - 笔触风格
    if (SUCCEEDED(hr)) {
        hr = safe_recreate(m_pAsStrokeStyle);
    }
    // 重建资源对象 - 笔触笔刷
    if (SUCCEEDED(hr)) {
        hr = safe_recreate(m_pAsBrushStroke);
    }
    // 重建资源对象 - 填充笔刷
    if (SUCCEEDED(hr)) {
        hr = safe_recreate(m_pAsBrushFill);
    }
    // 创建本类使用的设备接口资源
    if (SUCCEEDED(hr)) {
        hr = this->reset_asset();
    }
    // 返回重建结果
    return hr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Vector"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Vector::~Vector() noexcept {
    // 资源
    RubyGM::SafeRelease(m_pAsStrokeStyle);
    RubyGM::SafeRelease(m_pAsBrushStroke);
    RubyGM::SafeRelease(m_pAsBrushFill);
    // 接口
    RubyGM::SafeRelease(m_pGiStrokeStyle);
    RubyGM::SafeRelease(m_pGiBrushStroke);
    RubyGM::SafeRelease(m_pGiBrushFill);
}

// LINE
#include <core/drawable/rgmLine.h>

/// <summary>
/// Initializes a new instance of the <see cref="Line"/> class.
/// </summary>
/// <param name="ls">The ls.</param>
RubyGM::Drawable::Line::Line(const LineStatus& ls) noexcept:
Super(ls),
point0(ls.point0),
point1(ls.point1){

}

/// <summary>
/// Finalizes an instance of the <see cref="Line"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Line::~Line() noexcept {

}


/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Line::dispose() noexcept {
    this->Line::~Line();
    RubyGM::NormalFree(this);
}

/// <summary>
/// Creates the specified ls.
/// </summary>
/// <param name="ls">The ls.</param>
/// <returns></returns>
auto RubyGM::Drawable::Line::Create(const LineStatus& ls) noexcept ->Line* {
    auto ptr = RubyGM::NormalAlloc(sizeof(Line));
    if(!ptr) nullptr;
    return new(ptr) Line(ls);
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Line::Render(IGMRenderContext& rc) const noexcept {
    // 笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawLine(
            impl::d2d(point0),
            impl::d2d(point1),
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
    // 线段不支持填充
}
