#define _WIN32_WINNT 0x0A000001
#include <core/asset/rgmAssetStroke.h>
#include <core/graphics/rgmGraphics.h>
#include <core/drawable/vector/rgmVector.h>
#include <core/util/rgmImpl.h>
#include <core/rubygm.h>
#include <game/rgmGame.h>
#include <bridge/rgmluiBridge.h>


// rubygm namespace
namespace RubyGM {
    // bridge
    namespace Bridge {
        // Creates the path geometry.
        auto CreatePathGeometry(ID2D1PathGeometry*& path) noexcept->Result;
        // paerse svg path string
        auto SvgPathGeometry(
            const char* str, 
            ID2D1PathGeometry1*& path
        ) noexcept->Result;
        // Creates the filled geometry realization.
        auto CreateFilledGeometryRealization(
            ID2D1Geometry* geometry,
            float flatteningTolerance,
            ID2D1GeometryRealization **geometryRealization
        ) noexcept->Result;
        // Creates the stroked geometry realization.
        auto CreateStrokedGeometryRealization(
            ID2D1Geometry *geometry,
            FLOAT flatteningTolerance,
            FLOAT strokeWidth,
            ID2D1StrokeStyle *strokeStyle,
            ID2D1GeometryRealization **geometryRealization
        ) noexcept->Result;
    }
}


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
filled_color(vs.filled_color),
m_pAsStrokeStyle(vs.stroke_style),
m_pAsBrushStroke(vs.stroke_brush),
m_pAsBrushFilled(vs.filled_brush),
stroke_width(vs.stroke_width),
m_bUseStrokeColor(!vs.stroke_brush),
m_bUseFilledColor(!vs.filled_brush){
    // 重置相关资源图形接口数据
    this->reset_asset();
    // 使用特定笔刷 -> 设置颜色alpha到1.f
    if (vs.stroke_brush) this->stroke_color.a = 1.f;
    if (vs.filled_brush) this->filled_color.a = 1.f;
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
    RubyGM::SafeRelease(m_pGiBrushFilled);
    // 获取笔触资源
    m_pGiBrushStroke = m_pAsBrushStroke ?
        m_pAsBrushStroke->GetBrush() : Bridge::GetCommonBrush();
    // 获取填充资源
    m_pGiBrushFilled = m_pAsBrushFilled ?
        m_pAsBrushFilled->GetBrush() : Bridge::GetCommonBrush();
    // 获取笔触风格的图形接口数据 -- 本项允许null
    if (m_pAsStrokeStyle) {
        m_pGiStrokeStyle = m_pAsStrokeStyle->GetStrokeStyle();
    }
    // 返回结果
    return (m_pGiBrushStroke && m_pGiBrushFilled) ?
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
/// Set fill brushes, call this before draw filled
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Vector::set_filled_brush() const noexcept {
    if (m_bUseFilledColor) {
        impl::d2d<ID2D1SolidColorBrush>(
            m_pGiBrushFilled, IID_ID2D1SolidColorBrush
            )->SetColor(&impl::d2d(this->filled_color));
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
        hr = safe_recreate(m_pAsBrushFilled);
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
    RubyGM::SafeRelease(m_pAsBrushFilled);
    // 接口
    RubyGM::SafeRelease(m_pGiStrokeStyle);
    RubyGM::SafeRelease(m_pGiBrushStroke);
    RubyGM::SafeRelease(m_pGiBrushFilled);
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
point1(ls.point1) {
    this->filled_color.a = 0.f;
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
    // 填充
    if (this->is_draw_filled()) {
        this->set_filled_brush();
        rc.FillRoundedRectangle(rrc, m_pGiBrushFilled);
    }
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
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::RtRect::Render(IGMRenderContext& rc) const noexcept {
    // 填充
    if (this->is_draw_filled()) {
        this->set_filled_brush();
        rc.FillRectangle(&impl::d2d(rd_rect.rect), m_pGiBrushFilled);
    }
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
    // 填充
    if (this->is_draw_filled()) {
        this->set_filled_brush();
        rc.FillEllipse(ell, m_pGiBrushFilled);
    }
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
    // 填充
    if (this->is_draw_filled()) {
        this->set_filled_brush();
        rc.FillEllipse(ell, m_pGiBrushFilled);
    }
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
}

// ============================================================================
// ============================= Realization ==================================
// ============================================================================


// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    // status for Realization
    struct RealizationStatus : VectorStatus {
        // source geometry
        IGMGeometry*                source;
        // ctor
        RealizationStatus(Default v) : VectorStatus(v) {
            source = nullptr;
        }
    };
    // geometry realization
    struct Realization final : Vector {
    private:
        // super class
        using Super = Vector;
        // dispose
        void dispose() noexcept override;
        // reset
        auto reset_realization() noexcept->Result;
    protected:
        // ctor
        Realization(RealizationStatus& rs) noexcept;
        // dtor
        ~Realization() noexcept;
        // recreate
        auto recreate() noexcept->Result;
        // is ok
        bool is_ok() const noexcept {
            bool a = this->is_draw_stroke() == (!!m_pStrokeRealization);
            bool b = this->is_draw_filled() == (!!m_pFilledRealization);
            return a && b;
        }
    public:
        // create this
        static auto Create(RealizationStatus& rs) noexcept->Realization*;
        // render this
        void Render(IGMRenderContext& rc) const noexcept override;
    protected:
        // source geometry
        IGMGeometry*                m_pSourceGeometry = nullptr;
        // stroke realization
        ID2D1GeometryRealization*   m_pStrokeRealization = nullptr;
        // filled realization
        ID2D1GeometryRealization*   m_pFilledRealization = nullptr;
        // flattening tolerance
        float                       m_fFlatteningTolerance = 0.25f;
        // unused
        float                       m_unused_float = 233.f;
    };
}}

/// <summary>
/// Initializes a new instance of the <see cref="Realization"/> struct.
/// </summary>
/// <param name="vs">The vs.</param>
RubyGM::Drawable::Realization::Realization(RealizationStatus & rs) noexcept:
m_pSourceGeometry(RubyGM::SafeAcquire(rs.source)), 
m_fFlatteningTolerance(rs._unused_), Super(rs) {
    // 重置对象
    this->reset_realization();
}


/// <summary>
/// Creates the specified vs.
/// </summary>
/// <param name="vs">The vs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Realization::Create(
    RealizationStatus& rs) noexcept -> Realization* {
    assert(rs.source && "bad argument");
    if(!rs.source) return nullptr;
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Realization))) {
        // 创建对象
        auto obj = new(ptr) Realization(rs);
        // 创建成功
        if (obj->is_ok()) return obj;
        // 释放对象
        obj->dispose();
    }
    // 错误
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Realization"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Realization::~Realization() noexcept {
    RubyGM::SafeRelease(m_pSourceGeometry);
    RubyGM::SafeRelease(m_pStrokeRealization);
    RubyGM::SafeRelease(m_pFilledRealization);
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Realization::dispose() noexcept {
    this->Realization::~Realization();
    RubyGM::SmallFree(this);
}


/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Realization::recreate() noexcept -> Result {
    Result hr = Result(S_OK);
    // 重建父类
    if (SUCCEEDED(hr)) {
        hr = Super::recreate();
    }
    // 重建本类
    if (SUCCEEDED(hr)) {
        hr = this->reset_realization();
    }
    return hr;
}


/// <summary>
/// Resets this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Realization::reset_realization() noexcept -> Result {
    Result hr = Result(S_OK);
    // Realization对象
    ID2D1GeometryRealization* filled = nullptr;
    ID2D1GeometryRealization* stroke = nullptr;
    // 笔触
    if (SUCCEEDED(hr) && this->is_draw_stroke()) {
        // 创建对象
        hr = Bridge::CreateFilledGeometryRealization(
            m_pSourceGeometry,
            m_fFlatteningTolerance,
            &filled
        );
    }
    // 填充
    if (SUCCEEDED(hr) && this->is_draw_filled()) {
        // 创建对象
        hr = Bridge::CreateStrokedGeometryRealization(
            m_pSourceGeometry,
            m_fFlatteningTolerance,
            this->stroke_width,
            m_pGiStrokeStyle,
            &stroke
        );
    }
    // 嫁接对象
    RubyGM::SafeRelease(m_pFilledRealization);
    RubyGM::SafeRelease(m_pStrokeRealization);
    m_pFilledRealization = filled;
    m_pStrokeRealization = stroke;
    return hr;
}

/// <summary>
/// Renders the specified render context.
/// </summary>
/// <param name="rc">The  render context.</param>
/// <returns></returns>
void RubyGM::Drawable::Realization::Render(IGMRenderContext& rc)const noexcept{
    // 填充
    if (this->is_draw_filled() && m_pFilledRealization) {
        this->set_filled_brush();
        rc.DrawGeometryRealization(m_pFilledRealization, m_pGiBrushStroke);
    }
    // 笔触
    if (this->is_draw_stroke() && m_pFilledRealization) {
        this->set_stroke_brush();
        rc.DrawGeometryRealization(m_pStrokeRealization, m_pGiBrushStroke);
    }
}


// ============================================================================
// =============================== Geometry ===================================
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
    // 填充
    if (this->is_draw_filled()) {
        this->set_filled_brush();
        rc.FillGeometry(m_pGiGeometry, m_pGiBrushFilled);
    }
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
}

/// <summary>
/// Realizations the specified sf.
/// </summary>
/// <param name="sf">The sf.</param>
/// <returns></returns>
auto RubyGM::Drawable::Geometry::Realization(float sf) const noexcept->Vector* {
    assert(sf > 0.f && "bad acrgment"); assert(m_pGiGeometry && "bad action");
    if (sf <= 0.f) sf = 1.f;
    // 没有生成的理由
    if (!this->is_draw_filled() && !this->is_draw_stroke()) return nullptr;
    // 计算系数
    constexpr float FLATTENING_TOLERANCE = D2D1_DEFAULT_FLATTENING_TOLERANCE;
    float flattening_tolerance = FLATTENING_TOLERANCE / sf;
    // 创建对象
    RealizationStatus rs(Drawable::Default{});
    // Realization
    rs.source = m_pGiGeometry;
    // Vector
    rs.stroke_color = this->stroke_color;
    rs.filled_color = this->filled_color;
    rs.stroke_style = m_pAsStrokeStyle;
    rs.stroke_brush = m_pAsBrushStroke;
    rs.filled_brush = m_pAsBrushFilled;
    rs.stroke_width = this->stroke_width;
    rs._unused_ = flattening_tolerance;
    // Base
    return Drawable::Realization::Create(rs);
}

// ============================================================================
// =============================== Polygon ====================================
// ============================================================================
#include "core/drawable/vector/rgmPolygon.h"

/// <summary>
/// Initializes a new instance of the <see cref="Polygon"/> class.
/// </summary>
/// <param name="">The .</param>
RubyGM::Drawable::Polygon::Polygon(const PolygonStatus& ps) noexcept : 
Super(ps) {
    assert(m_pGiGeometry == nullptr);
    assert(ps.points && ps.count >= 3); 
    // 初始化
    ID2D1PathGeometry* path = nullptr;
    ID2D1GeometrySink* sink = nullptr;
    // 创建路径几何
    auto hr = Bridge::CreatePathGeometry(path);
    // 打开路径记录
    if (SUCCEEDED(hr)) {
        hr = path->Open(&sink);
    }
    // 填充路径
    if (SUCCEEDED(hr)) {
        auto points = impl::d2d(ps.points);
        sink->SetFillMode(D2D1_FILL_MODE(ps.fill_rule));
        sink->BeginFigure(*points, D2D1_FIGURE_BEGIN_FILLED);
        sink->AddLines(points + 1, ps.count - 1);
        sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        hr = sink->Close();
    }
    // 错误代码
    if (FAILED(hr)) Game::SetLastErrorCode(hr);
    RubyGM::SafeRelease(sink);
    // 数据转换
    auto gi = static_cast<ID2D1Geometry*>(path);
    m_pGiGeometry = static_cast<IGMGeometry*>(gi);
}


/// <summary>
/// Creates the specified ps.
/// </summary>
/// <param name="ps">The ps.</param>
/// <returns></returns>
auto RubyGM::Drawable::Polygon::Create(
    const PolygonStatus& ps) noexcept -> Polygon * {
    assert(ps.points && ps.count >= 3 && "bad argment"); 
    // 参数无效
    if (ps.points == nullptr || ps.count < 3) return nullptr; 
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Polygon))) {
        // 初始化对象
        auto obj = new(ptr) Polygon(ps);
        // 初始化成功
        if (obj->is_ok()) return obj;
        // 失败则释放对象
        obj->dispose();
    }
    // 各种失败
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Polygon"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Polygon::~Polygon() noexcept {
}


/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Polygon::dispose() noexcept {
    this->Polygon::~Polygon();
    RubyGM::SmallFree(this);
}


// ============================================================================
// =============================== Polyline ===================================
// ============================================================================
#include "core/drawable/vector/rgmPolyline.h"

/// <summary>
/// Initializes a new instance of the <see cref="Polyline"/> class.
/// </summary>
/// <param name="">The .</param>
RubyGM::Drawable::Polyline::Polyline(const PolylineStatus& ps) noexcept : 
Super(ps) {
    assert(m_pGiGeometry == nullptr);
    assert(ps.points && ps.count >= 2); 
    this->filled_color.a = 0.f;
    // 初始化
    ID2D1PathGeometry* path = nullptr;
    ID2D1GeometrySink* sink = nullptr;
    // 创建路径几何
    auto hr = Bridge::CreatePathGeometry(path);
    // 打开路径记录
    if (SUCCEEDED(hr)) {
        hr = path->Open(&sink);
    }
    // 填充路径
    if (SUCCEEDED(hr)) {
        auto points = impl::d2d(ps.points);
        sink->BeginFigure(*points, D2D1_FIGURE_BEGIN_HOLLOW);
        sink->AddLines(points + 1, ps.count - 1);
        sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        hr = sink->Close();
    }
    // 错误代码
    if (FAILED(hr)) Game::SetLastErrorCode(hr);
    RubyGM::SafeRelease(sink);
    // 数据转换
    auto gi = static_cast<ID2D1Geometry*>(path);
    m_pGiGeometry = static_cast<IGMGeometry*>(gi);
}


/// <summary>
/// Creates the specified ps.
/// </summary>
/// <param name="ps">The ps.</param>
/// <returns></returns>
auto RubyGM::Drawable::Polyline::Create(
    const PolylineStatus& ps) noexcept -> Polyline * {
    assert(ps.points && ps.count >= 2 && "bad argment"); 
    // 参数无效
    if (ps.points == nullptr || ps.count < 2) return nullptr; 
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Polyline))) {
        // 初始化对象
        auto obj = new(ptr) Polyline(ps);
        // 初始化成功
        if (obj->is_ok()) return obj;
        // 失败则释放对象
        obj->dispose();
    }
    // 各种失败
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Polyline"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Polyline::~Polyline() noexcept {
}


/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Polyline::dispose() noexcept {
    this->Polyline::~Polyline();
    RubyGM::SmallFree(this);
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The Render Context.</param>
/// <returns></returns>
void RubyGM::Drawable::Polyline::Render(IGMRenderContext& rc) const noexcept {
    // 只用画笔触
    if (this->is_draw_stroke()) {
        this->set_stroke_brush();
        rc.DrawGeometry(
            m_pGiGeometry,
            m_pGiBrushStroke,
            this->stroke_width,
            m_pGiStrokeStyle
        );
    }
}

// ============================================================================
// ================================= Path =====================================
// ============================================================================
#include "core/drawable/vector/rgmPath.h"

/// <summary>
/// Initializes a new instance of the <see cref="Path"/> class.
/// </summary>
/// <param name="">The .</param>
RubyGM::Drawable::Path::Path(const PathStatus& ps) noexcept : Super(ps) {
    assert(m_pGiGeometry == nullptr);
    // 初始化
    ID2D1PathGeometry1* path = nullptr;
    // 创建路径几何
    auto hr = Bridge::SvgPathGeometry(ps.path, path);
    // 错误代码
    if (FAILED(hr)) Game::SetLastErrorCode(hr);
    // 数据转换
    auto gi = static_cast<ID2D1Geometry*>(path);
    m_pGiGeometry = static_cast<IGMGeometry*>(gi);
}


/// <summary>
/// Creates the specified ps.
/// </summary>
/// <param name="ps">The ps.</param>
/// <returns></returns>
auto RubyGM::Drawable::Path::Create(const PathStatus& ps) noexcept ->Path* {
    assert(ps.path && "bad argment"); 
    // 参数无效
    if (ps.path == nullptr) return nullptr; 
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Path))) {
        // 初始化对象
        auto obj = new(ptr) Path(ps);
        // 初始化成功
        if (obj->is_ok()) return obj;
        // 失败则释放对象
        obj->dispose();
    }
    // 各种失败
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Path"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Path::~Path() noexcept {
}


/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Path::dispose() noexcept {
    this->Path::~Path();
    RubyGM::SmallFree(this);
}

