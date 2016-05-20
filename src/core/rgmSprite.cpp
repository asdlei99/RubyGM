// M$
#include <core/rubygm.h>
#include <core/graphics/rgmSprite.h>
#include <core/graphics/rgmGraphics.h>
#include <core/util/rgmImpl.h>


// RubyGM namespace
namespace RubyGM {
    // default sprite status
    const SprteStatus DEFAULT_STATUS = {
        0.f, 0.f,
        1.f, 1.f,
        0.f, 0.f,
        0.f, 0.f,
        0.f, 0
    };
}

/// <summary>
/// Sets the drawable.
/// </summary>
/// <param name="obj">The object.</param>
/// <returns></returns>
void RubyGM::CGMSprite::SetDrawable(CGMPtrA<Drawable::Object>&& obj) noexcept {
    m_spDrawable = std::move(obj);
}

/// <summary>
/// Sets the drawable.
/// </summary>
/// <param name="obj">The object.</param>
/// <returns></returns>
void RubyGM::CGMSprite::SetDrawable(const CGMPtrA<Drawable::Object>& obj) noexcept {
    m_spDrawable = obj;
}

/// <summary>
/// Initializes a new instance of the <see cref="CGMSprite"/> class.
/// </summary>
RubyGM::CGMSprite::CGMSprite(const SprteStatus& ss, CGMSprite* parent) noexcept
    :m_pParent(parent), m_status(ss) {
    this->make_transform(m_matWorld);
    this->set_visible();
    this->ClearClipRect();
}

/// <summary>
/// Finalizes an instance of the <see cref="CGMSprite"/> class.
/// </summary>
/// <returns></returns>
RubyGM::CGMSprite::~CGMSprite() noexcept {

}

/// <summary>
/// Computes the scale factor1 .
/// </summary>
/// <returns></returns>
auto RubyGM::CGMSprite::ComputeScaleFactorEz1() const noexcept -> float {
    auto sf = this->ComputeScaleFactorEz2();
    return (sf.width + sf.height) * 0.5f;
}

/// <summary>
/// Computes the scale factor ex1.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMSprite::ComputeScaleFactorEx1() const noexcept -> float {
    auto sf = this->ComputeScaleFactorEx2();
    return (sf.width + sf.height) * 0.5f;
}


/// <summary>
/// Computes the scale factor2.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMSprite::ComputeScaleFactorEz2() const noexcept -> SizeF {
    constexpr float pi = 3.141592654f;
    auto sprite = this;
    SizeF sf{ 1.f, 1.f };
    // 遍历节点
    while (sprite) {
        sf.width *= sprite->GetZoomX();
        sf.height *= sprite->GetZoomY();
        sprite = sprite->m_pParent;
    }
    // 返回结果
    return sf;
}

/// <summary>
/// Computes the scale factor ex2.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMSprite::ComputeScaleFactorEx2() const noexcept -> SizeF {
    constexpr float pi = 3.141592654f;
    auto sprite = this;
    SizeF sf{ 1.f, 1.f };
    // 遍历节点
    while (sprite) {
        sf.width *= sprite->GetZoomX();
        sf.height *= sprite->GetZoomY();
        if (sprite->GetSkewX() != 0.f) {
            float theta = sprite->GetSkewX() * (pi / 180.0f);
            sf.width *= std::tan(theta) / theta;
        }
        if (sprite->GetSkewY() != 0.f) {
            float theta = sprite->GetSkewY() * (pi / 180.0f);
            sf.height *= std::tan(theta) / theta;
        }
        sprite = sprite->m_pParent;
    }
    // 返回结果
    return sf;
}

/// <summary>
/// Clears this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMSprite::Clear() noexcept {
    m_ltChildren.clear();
    m_spDrawable.Dispose();
}


/// <summary>
/// Gets the specified .
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::CGMSprite::Get(SprteStatus& status) const noexcept {
    status = m_status;
}

/// <summary>
/// Sets the specified .
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::CGMSprite::Set(const SprteStatus& status) noexcept {
    auto zchanged = m_status.z != status.z;
    m_status = status;
    this->set_world_changed();
    this->set_childz_changed(zchanged);
}

/// <summary>
/// Adds the child.
/// </summary>
/// <returns>the new child refence</returns>
auto RubyGM::CGMSprite::AddChild(const SprteStatus& ss) noexcept ->CGMSprite* {
    // throw(std::exception)*
    try {
        // 可以在尾部插入
        if (m_ltChildren.empty() ||
            ss.z >= m_ltChildren.back().m_status.z) {
            m_ltChildren.emplace_back(ss, this);
            return &m_ltChildren.back();
        }
        // 在中间插入
        for (auto itr = m_ltChildren.begin();
            itr != m_ltChildren.end(); ++itr) {
            if (itr->m_status.z > ss.z) {
                return &(*(m_ltChildren.emplace(itr, ss, this)));
            }
        }
        assert(!"BAD ACTION");
    }
    catch (...) { void(0); }
    return nullptr;
}


/// <summary>
/// Renders this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMSprite::Render(IGMRenderContext& rc) noexcept {
#ifdef _DEBUG
    {
        ID2D1DeviceContext* dc = nullptr;
        rc.QueryInterface(IID_ID2D1DeviceContext, reinterpret_cast<void**>(&dc));
        assert(dc && "bad interface!");
        RubyGM::SafeRelease(dc);
    }
#endif
    // 不可见
    if (!this->is_visible()) return;
    // 刷新
    this->update();
    // 设置转变矩阵
    rc.SetTransform(&impl::d2d(m_matWorld));
    // 裁剪
    this->push_clip(rc);
    // 选择本精灵
    if (m_spDrawable) m_spDrawable->Render(rc);
    // 渲染子精灵
    for (auto& child : m_ltChildren) child.Render(rc);
    // 裁剪
    this->pop_clip(rc);
}


/// <summary>
/// push clip rect for this instance.
/// </summary>
void RubyGM::CGMSprite::push_clip(IGMRenderContext& rc) noexcept {
    // 设置剪切矩形
    if (this->is_strict_clip()) {
        rc.PushLayer(
            D2D1::LayerParameters1(
                impl::d2d(m_rcClip),
                nullptr,
                D2D1_ANTIALIAS_MODE(this->antialias_mode)
            ), 
            nullptr
        );
    }
    else {
        rc.PushAxisAlignedClip(
            impl::d2d(m_rcClip), 
            D2D1_ANTIALIAS_MODE(this->antialias_mode)
        );
    }
}

/// <summary>
/// pop clip rect for this instance.
/// </summary>
void RubyGM::CGMSprite::pop_clip(IGMRenderContext& rc) noexcept {
    // 弹出剪切矩形
    if (this->is_strict_clip()) rc.PopLayer();
    else rc.PopAxisAlignedClip();
}


/// <summary>
/// Updates this instance.
/// </summary>
void RubyGM::CGMSprite::update() noexcept {
    // 必须要求
    assert(m_pParent && "no parent!");
    // 要求排序
    if (this->is_childz_changed()) {
        this->clear_childz_changed();
        this->sort_children();
    }
    // 检查世界转变矩阵
    if (this->is_world_changed()) {
        this->clear_world_changed();
        // 设置全部子精灵均需要
        for (auto& child : m_ltChildren) 
            child.set_world_changed();
        // 修改世界转换矩阵
        D2D1_MATRIX_3X2_F transform;
        this->make_transform(impl::rubygm(transform));
        m_matWorld = impl::rubygm(
            transform * impl::d2d(m_pParent->m_matWorld)
        );
    }
}

/// <summary>
/// Roots the render.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::CGMSprite::RootRender(IGMRenderContext& rc) const noexcept {
    // 设置转变
    //rc.SetTransform(&impl::d2d(m_matWorld));
    // 设置剪切矩形
    //rc.PushAxisAlignedClip(impl::d2d(m_rcClip), D2D1_ANTIALIAS_MODE(this->antialias_mode));
    // 渲染子精灵
    for (const auto& child : m_ltChildren) {
        // XXX: UNLEGAL
        const_cast<CGMSprite&>(child).Render(rc);
    }
    // 弹出剪切矩形
    //rc.PopAxisAlignedClip();
}

// rubygm::dx namespace
namespace RubyGM { namespace DX{
    /// <summary>
    /// D2s the d1 make rotate matrix.
    /// </summary>
    /// <param name="angle">The angle.</param>
    /// <param name="center">The center.</param>
    /// <param name="matrix">The matrix.</param>
    /// <returns></returns>
    inline void D2D1MakeRotateMatrix(float angle, D2D1_POINT_2F center, D2D1_MATRIX_3X2_F& matrix) noexcept {
        constexpr float pi = 3.141592654f;
        float theta = angle * (pi / 180.0f);
        float sin_theta = std::sin(theta);
        float cos_theta = std::cos(theta);
        matrix._11 = cos_theta;
        matrix._12 = sin_theta;
        matrix._21 = -sin_theta;
        matrix._22 = cos_theta;
        matrix._31 = center.x - center.x * cos_theta + center.y * sin_theta;
        matrix._32 = center.y - center.x * sin_theta - center.y * cos_theta;
    }
    // make skew
    inline void D2D1MakeSkewMatrix(
        float x, float y, 
        D2D1_POINT_2F center,
        D2D1_MATRIX_3X2_F& matrix) noexcept {
        constexpr float pi = 3.141592654f;
        float theta_x = x * (pi / 180.0f), tan_x = std::tan(theta_x);
        float theta_y = y * (pi / 180.0f), tan_y = std::tan(theta_y);
        matrix._11 = 1.f;              matrix._12 = tan_y;
        matrix._21 = tan_x;            matrix._22 = 1.f;
        matrix._31 =-center.y * tan_x; matrix._32 = center.x * tan_y;
    }
}}

/// <summary>
/// Make specified transform.
/// </summary>
/// <param name="transform">The transform.</param>
/// <returns></returns>
void RubyGM::CGMSprite::make_transform(Matrix3X2F& transform) const noexcept {
    auto& matrix = impl::d2d(transform);
    D2D1_MATRIX_3X2_F romt;
    D2D1_POINT_2F center{ m_status.x + m_status.ox, m_status.y + m_status.oy };
    RubyGM::DX::D2D1MakeRotateMatrix(m_status.rotation, center, romt);
    // 常规转变
    matrix = D2D1::Matrix3x2F::Translation(D2D1_SIZE_F{ m_status.x, m_status.y })
        * D2D1::Matrix3x2F::Scale(D2D1_SIZE_F{m_status.zoomx, m_status.zoomy}, center)
        * romt;
    // 倾斜转变 -- 不常用....
    if (m_status.skewx != 0.f || m_status.skewy != 0.f) {
        DX::D2D1MakeSkewMatrix(m_status.skewx, m_status.skewy, center, romt);
        matrix = matrix * romt;
    }
}

/// <summary>
/// Sort_childrens this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMSprite::sort_children() noexcept {
    // 在移动的时候发生异常时会抛出
    m_ltChildren.sort([](const CGMSprite& a, const CGMSprite& b) noexcept {
        return a.m_status.z < b.m_status.z;
    });
}

/// <summary>
/// Sets the transform.
/// </summary>
/// <param name="transform">The transform.</param>
/// <returns></returns>
void RubyGM::CGMSprite::SetTransform(const Matrix3X2F& transform) noexcept {
    m_matWorld = transform;
    this->clear_world_changed();
}
