#include <core/asset/rgmAssetStroke.h>
#include <core/graphics/rgmGraphics.h>
#include <core/drawable/vector/rgmVector.h>
#include <core/util/rgmImpl.h>
#include <core/rubygm.h>
#include <game/rgmGame.h>
#include <bridge/rgmluiBridge.h>

// ============================================================================
// =============================== Vector =====================================
// ============================================================================

/// <summary>
/// Initializes a new instance of the <see cref="Vector"/> class.
/// </summary>
/// <param name="vs">The vs.</param>
RubyGM::Drawable::Vector::Vector(const VectorStatus& vs) noexcept:
Super(vs),
stroke_color(vs.stroke_color),
fill_color(vs.fill_color),
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

// ============================================================================
// ================================ Line ======================================
// ============================================================================

#include <core/drawable/vector/rgmLine.h>

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


// ============================================================================
// ================================ Rect ======================================
// ============================================================================

#include <core/drawable/vector/rgmRect.h>

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    // always right angle
    struct RtRect final : Rect {
        // super class
        using Super = Rect;
        // ctor
        RtRect(const RectStatus& rs) noexcept : Super(rs) {}
        // render object
        void Render(IGMRenderContext& rc) const noexcept override;
    };
}}

/// <summary>
/// Initializes a new instance of the <see cref="Rect"/> class.
/// </summary>
/// <param name="rs">The ls.</param>
RubyGM::Drawable::Rect::Rect(const RectStatus& rs) noexcept:
Super(rs), rd_rect(rs.rd_rect) {

}

/// <summary>
/// Finalizes an instance of the <see cref="Rect"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Rect::~Rect() noexcept {

}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Rect::dispose() noexcept {
    this->Rect::~Rect();
    RubyGM::NormalFree(this);
}

/// <summary>
/// Creates the specified ls.
/// </summary>
/// <param name="ls">The ls.</param>
/// <returns></returns>
auto RubyGM::Drawable::Rect::Create(const RectStatus& rs) noexcept ->Rect* {
    auto ptr = RubyGM::NormalAlloc(sizeof(Rect));
    if(!ptr) nullptr;
    // 圆角矩形任意半径 < 0 -> 直角矩形
    if (rs.rd_rect.rx < 0.f || rs.rd_rect.ry < 0.f) {
        return new(ptr) RtRect(rs);
    }
    else {
        return new(ptr) Rect(rs);
    }
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Rect::Render(IGMRenderContext& rc) const noexcept {
    const auto rrc = reinterpret_cast<const D2D1_ROUNDED_RECT*>(&rd_rect);
    // 笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawRoundedRectangle(
            rrc,
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
    // 填充
    if (this->is_draw_fill()) {
        this->set_fill_brush();
        rc.FillRoundedRectangle(rrc, m_pGiBrushFill);
    }
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::RtRect::Render(IGMRenderContext& rc) const noexcept {
    // 笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawRectangle(
            &impl::d2d(rd_rect.rect),
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
    // 填充
    if (this->is_draw_fill()) {
        this->set_fill_brush();
        rc.FillRectangle(&impl::d2d(rd_rect.rect), m_pGiBrushFill);
    }
}

// ============================================================================
// =============================== Ellipse ====================================
// ============================================================================

#include <core/drawable/vector/rgmEllipse.h>

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    // always right angle
    struct Circle final : Ellipse {
        // super class
        using Super = Ellipse;
        // ctor
        Circle(const EllipseStatus& es) noexcept : Super(es) {}
        // render object
        void Render(IGMRenderContext& rc) const noexcept override;
    };
}}

/// <summary>
/// Initializes a new instance of the <see cref="Ellipse"/> class.
/// </summary>
/// <param name="es">The ls.</param>
RubyGM::Drawable::Ellipse::Ellipse(const EllipseStatus& es) noexcept:
Super(es), ellipse(es.ellipse) {

}

/// <summary>
/// Finalizes an instance of the <see cref="Ellipse"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Ellipse::~Ellipse() noexcept {

}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Ellipse::dispose() noexcept {
    this->Ellipse::~Ellipse();
    RubyGM::NormalFree(this);
}

/// <summary>
/// Creates the specified ls.
/// </summary>
/// <param name="ls">The ls.</param>
/// <returns></returns>
auto RubyGM::Drawable::Ellipse::Create(
    const EllipseStatus& es) noexcept ->Ellipse* {
    auto ptr = RubyGM::NormalAlloc(sizeof(Ellipse));
    if(!ptr) nullptr;
    // 椭圆Y轴半径 < 0 -> 圆
    if (es.ellipse.ry < 0.f) {
        return new(ptr) Circle(es);
    }
    else {
        return new(ptr) Ellipse(es);
    }
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Ellipse::Render(IGMRenderContext& rc) const noexcept {
    const auto ell = reinterpret_cast<const D2D1_ELLIPSE*>(&ellipse);
    // 笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawEllipse(
            ell,
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
    // 填充
    if (this->is_draw_fill()) {
        this->set_fill_brush();
        rc.FillEllipse(ell, m_pGiBrushFill);
    }
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Circle::Render(IGMRenderContext& rc) const noexcept {
    auto circle = this->ellipse;
    circle.ry = circle.rx;
    const auto ell = reinterpret_cast<const D2D1_ELLIPSE*>(&circle);
    // 笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawEllipse(
            ell,
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
    // 填充
    if (this->is_draw_fill()) {
        this->set_fill_brush();
        rc.FillEllipse(ell, m_pGiBrushFill);
    }
}

// ============================================================================
// =============================== Geometry ====================================
// ============================================================================
#include <core/drawable/vector/rgmGeometry.h>

/// <summary>
/// Finalizes an instance of the <see cref="Geometry"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Geometry::~Geometry() noexcept {
    RubyGM::SafeRelease(m_pGiGeometry);
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The Render Context.</param>
/// <returns></returns>
void RubyGM::Drawable::Geometry::Render(IGMRenderContext& rc) const noexcept {
    // 笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawGeometry(
            m_pGiGeometry,
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
    // 填充
    if (this->is_draw_fill()) {
        this->set_fill_brush();
        rc.FillGeometry(m_pGiGeometry, m_pGiBrushFill);
    }
}