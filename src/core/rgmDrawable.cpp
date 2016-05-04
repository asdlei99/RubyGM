#include <core/rubygm.h>
#include <core/graphics/rgmDrawable.h>
#include <core/graphics/rgmGraphics.h>
#include <core/graphics/Drawable/rgmLine.h>

// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // d2d
    auto d2d(const RubyGM::Point2F& pt) { return reinterpret_cast<const D2D1_POINT_2F&>(pt); }
    // rubygm
    auto rubygm(const D2D1_POINT_2F& pt) { return reinterpret_cast<const RubyGM::Point2F&>(pt); }
    // d2d
    auto d2d(const RubyGM::ColorF& c) { return reinterpret_cast<const D2D1_COLOR_F&>(c); }
    // rubygm
    auto rubygm(const D2D1_COLOR_F& c) { return reinterpret_cast<const RubyGM::ColorF&>(c); }
    // ---------------------------
    template<class T, class Y, class U>
    auto d2d(Y* obj, const U& iid) noexcept -> T* {
#ifdef _DEBUG
        if(obj) {
            void* brush = nullptr;
            assert(SUCCEEDED(obj->QueryInterface(iid, &brush)));
        }
#endif
        auto super = static_cast<typename Y::Super*>(obj);
        return static_cast<T*>(super);
    }
}}



/// <summary>
/// Recreates all Drawable object.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Base::RecreateAll() noexcept ->uint32_t {
    // 获取最后那个
    auto obj = Drawable::GetLastDrawableObject()->m_pPrve;
    uint32_t hr = uint32_t(S_OK);
    // 遍历链表
    while (obj->m_pPrve) {
        hr = obj->RecreateAll();
        if (FAILED(HRESULT(hr))) break;
        obj = obj->m_pPrve;
    }
    // 返回结果
    return hr;
}

/// <summary>
/// Gets the color brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::GetColorBrush() noexcept ->GMBrush* {
    return nullptr;
}

/// <summary>
/// Releases ref-count for this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Base::Release() noexcept -> uint32_t {
    uint32_t count = --m_cRef; 
    if (!count) this->dispose(); 
    return count; 
}


/// <summary>
/// Finalizes an instance of the <see cref="Base"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Base::~Base() noexcept {
    RubyGM::SafeRelease(m_pBrush);
    // 前面链接后面
    m_pPrve->m_pNext = m_pNext;
    // 后面链接前面
    m_pNext->m_pPrve = m_pPrve;
}

/// <summary>
/// Finalizes an instance of the <see cref="Base"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Base::Base() noexcept {
    // 初始化颜色
    m_color.b = m_color.g = m_color.r = 0.f; m_color.a = 1.f;
    // 设置链表指针
    auto last = Drawable::GetLastDrawableObject();
    assert(last);
    // 链接后面
    m_pNext = last;
    // 链接前面
    m_pPrve = last->m_pPrve;
    // 前面链接自己
    m_pPrve->m_pNext = this;
    // 后面链接自己
    m_pNext->m_pPrve = this;
}

/// <summary>
/// Sets the color.
/// </summary>
/// <param name="color">The color.</param>
/// <returns></returns>
void RubyGM::Drawable::Base::SetColor(const RubyGM::ColorF& color) noexcept {
    // 非纯色笔刷则释放旧的笔刷
    if (m_modeBrush != BrushMode::Mode_Color) {
        RubyGM::SafeRelease(m_pBrush);
        m_pBrush = Drawable::GetColorBrush();
        m_modeBrush = BrushMode::Mode_Color;
    }
    // 修改颜色
    m_color = color;
}



/// <summary>
/// Sets the brush.
/// </summary>
/// <param name="brush">The brush.</param>
/// <returns></returns>
void RubyGM::Drawable::Base::SetBrush(GMBrush* brush) noexcept {
    assert(brush && "bad brush");
    RubyGM::SafeRelease(m_pBrush);
    m_pBrush = RubyGM::SafeAcquire(brush);
}



// recreate
auto RubyGM::Drawable::Base::Recreate() noexcept -> uint32_t {
    return uint32_t(S_OK);
}

/// <summary>
/// Before_renders this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Base::before_render() const noexcept {
    // 纯色笔刷
    if (m_modeBrush == BrushMode::Mode_Color) {
#ifdef _DEBUG
        {
            void* brush = nullptr;
            assert(SUCCEEDED(m_pBrush->QueryInterface(IID_ID2D1SolidColorBrush, &brush)));
        }
#endif
        auto brush = impl::d2d<ID2D1SolidColorBrush>(m_pBrush, IID_ID2D1SolidColorBrush);
        brush->SetColor(impl::d2d(m_color));
    }
}


// ------------------- Line ------------------

/// <summary>
/// Initializes a new instance of the <see cref="Line"/> class.
/// </summary>
/// <param name="mode">The mode.</param>
RubyGM::Drawable::Line::Line() noexcept : Super() {
    point0 = { 0.f, 0.f };
    point1 = { 0.f, 0.f };
}


/// <summary>
/// Finalizes an instance of the <see cref="Line"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Line::~Line() noexcept {
}

/// <summary>
/// Creates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Line::Create() noexcept -> Line* {
    return new(std::nothrow) Line;
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Line::dispose() noexcept {
    delete this;
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Line::Render(GMRednerContext& rc) const noexcept {
    assert(m_pBrush && "bad brush");
    this->before_render();
    rc.DrawLine(
        impl::d2d(this->point0), 
        impl::d2d(this->point1), 
        m_pBrush, 
        this->stroke_width,
        nullptr
    );
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Line::Recreate() noexcept -> uint32_t {
    return uint32_t(S_OK);
}

