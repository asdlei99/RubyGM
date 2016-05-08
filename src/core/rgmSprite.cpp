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
        0.f
    };
}

/// <summary>
/// Initializes a new instance of the <see cref="CGMSprite"/> class.
/// </summary>
RubyGM::CGMSprite::CGMSprite(CGMSprite* parent) noexcept :m_pParent(parent) {
    this->make_transform(m_matWorld);
    this->set_visible();
    this->ClearClipRect();
    m_status = DEFAULT_STATUS;
}

/// <summary>
/// Finalizes an instance of the <see cref="CGMSprite"/> class.
/// </summary>
/// <returns></returns>
RubyGM::CGMSprite::~CGMSprite() noexcept {

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
     m_status = status;
     this->set_world_changed();
}

/// <summary>
/// Adds the child.
/// </summary>
/// <returns>the new child refence</returns>
auto RubyGM::CGMSprite::AddChild() /*throw(std::exception)*/ ->CGMSprite& {
    m_ltChildren.emplace_back(this);
    using titr = decltype(m_ltChildren)::iterator;
    using vtype = decltype(m_ltChildren)::value_type;
    return m_ltChildren.back();
}


/// <summary>
/// Renders this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMSprite::Render(IGMRednerContext& rc) noexcept {
    // 必须要求
    assert(m_pParent && "no parent!");
    // 检查世界转变矩阵
    if (this->is_world_changed()) {
        this->clear_world_changed();
        this->make_transform(m_matWorld);
    }
    // 不可见
    if (!this->is_visible()) return;
    // 设置转变
    const auto& mt = impl::d2d(m_pParent->m_matWorld);
    rc.SetTransform(mt * impl::d2d(m_matWorld));
    // 设置剪切矩形
    rc.PushAxisAlignedClip(impl::d2d(m_rcClip), D2D1_ANTIALIAS_MODE(this->antialias_mode));
    // 有效的可刻画物体
    if (m_spDrawable) m_spDrawable->Render(rc);
    // 渲染子精灵
    for (auto& child : m_ltChildren) child.Render(rc);
    // 弹出剪切矩形
    rc.PopAxisAlignedClip();
}

/// <summary>
/// Roots the render.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::CGMSprite::RootRender(IGMRednerContext& rc) const noexcept {
    // 设置转变
    rc.SetTransform(&impl::d2d(m_matWorld));
    // 设置剪切矩形
    rc.PushAxisAlignedClip(impl::d2d(m_rcClip), D2D1_ANTIALIAS_MODE(this->antialias_mode));
    // 渲染子精灵
    for (const auto& child : m_ltChildren) {
        // XXX: UNLEGAL
        const_cast<CGMSprite&>(child).Render(rc);
    }
    // 弹出剪切矩形
    rc.PopAxisAlignedClip();
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
    void D2D1MakeRotateMatrix(float angle, D2D1_POINT_2F center, D2D1_MATRIX_3X2_F& matrix) noexcept {
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
}}

/// <summary>
/// Make specified transform.
/// </summary>
/// <param name="transform">The transform.</param>
/// <returns></returns>
void RubyGM::CGMSprite::make_transform(Matrix3X2F& transform) const noexcept {
    auto& matrix = impl::d2d(transform);
    D2D1_MATRIX_3X2_F romt;
    D2D1_POINT_2F center = { m_status.x + m_status.ox, m_status.y + m_status.oy };
    RubyGM::DX::D2D1MakeRotateMatrix(m_status.rotation, center, romt);
    matrix = D2D1::Matrix3x2F::Translation(D2D1::SizeF(m_status.x, m_status.y))
        * D2D1::Matrix3x2F::Scale(D2D1::SizeF(m_status.zoomx, m_status.zoomy), center)
        * romt;
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
