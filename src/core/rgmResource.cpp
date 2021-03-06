﻿#define _WIN32_WINNT 0x0A000001
#include <core/drawable/rgmBitmap.h>
#include <core/asset/rgmAssetFont.h>
#include <core/graphics/rgmGraphics.h>
#include <core/text/rgmTextDetail.h>
#include <core/util/rgmUtil.h>
#include <game/rgmGame.h>
#include <bridge/rgmluiBridge.h>
#include <cwchar>
#include <memory>


/// <summary>
/// Rasterizations the specified sf.
/// </summary>
/// <param name="sf">The scale factor.</param>
/// <param name="bs">The basic size.</param>
/// <returns></returns>
auto RubyGM::Drawable::Object::Rasterization(
    SizeF sf, SizeF bs) noexcept -> Drawable::Bitmap* {
    auto asset = Game::CreateBitmapAssetFromDrawable(this, sf, bs);
    Drawable::BitmapStatus bst(std::move(asset));
    // 创建成功
    if (const auto bsp = Drawable::Bitmap::Create(bst)) {
        bsp->des_rect.right = bs.width;
        bsp->des_rect.bottom = bs.height;
        return bsp;
    }
    return nullptr;
}

/// <summary>
/// Initializes a new instance of the <see cref="Resource"/> class.
/// </summary>
RubyGM::Base::Resource::Resource() noexcept {
    // 设置链表指针
    auto last = Bridge::GetLastResourceObject();
    assert(last);
    // 链接后面
    this->next = last;
    // 链接前面
    this->prve = last->prve;
    // 前面链接自己
    this->prve->next = this;
    // 后面链接自己
    this->next->prve = this;
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Base::Resource::Recreate() noexcept -> Result {
    // 可以重建
    if (m_bCouldRecreate) {
        m_bCouldRecreate = false;
        return this->recreate();
    } 
    // 已经重建完毕
    else {
        return Result(S_OK);
    }
}

/// <summary>
/// Finalizes an instance of the <see cref="Resource"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Base::Resource::~Resource() noexcept {
    // 前面链接自己
    this->prve->next = this->next;
    // 后面链接自己
    this->next->prve = this->prve;
}

/// <summary>
/// Adds the reference.
/// </summary>
/// <returns></returns>
auto RubyGM::Base::Resource::AddRef() noexcept -> uint32_t {
    assert(m_cRef < 1024 && "high ref-count");
    return ++m_cRef;
}

/// <summary>
/// Releases this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Base::Resource::Release() noexcept -> uint32_t {
    assert(m_cRef < 1024 && "high ref-count");
    uint32_t count = --m_cRef; 
    if (!count) this->dispose(); 
    return count; 
}

// drawable
//#include <core/drawable/rgmDrawable.h>
//#include <core/util/rgmImpl.h>

#if 0

/// <summary>
/// Initializes a new instance of the <see cref="Object"/> class.
/// </summary>
/// <param name="">The .</param>
RubyGM::Drawable::Object::Object(const BaseStatus& bs) noexcept
: m_color(bs.color), m_pBrush(Game::GetCommonBrush()) { }


/// <summary>
/// Finalizes an instance of the <see cref="Object"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Object::~Object() noexcept {
    // 释放笔刷
    RubyGM::SafeRelease(m_pBrush);
}

/// <summary>
/// Sets the color.
/// </summary>
/// <param name="color">The color.</param>
/// <returns></returns>
void RubyGM::Drawable::Object::SetColor(const RubyGM::ColorF& color) noexcept {
    // 非纯色笔刷则释放旧的笔刷
    if (this->get_brush_mode() != BrushMode::Mode_Color) {
        RubyGM::SafeRelease(m_pBrush);
        m_pBrush = Game::GetCommonBrush();
        this->set_brush_mode(BrushMode::Mode_Color);
    }
    // 修改颜色
    m_color = color;
}


/// <summary>
/// Sets the brush.
/// </summary>
/// <param name="brush">The brush.</param>
/// <returns></returns>
void RubyGM::Drawable::Object::SetBrush(IGMBrush* brush) noexcept {
    assert(brush && "bad brush");
    RubyGM::SafeRelease(m_pBrush);
    m_pBrush = RubyGM::SafeAcquire(brush);
    this->set_brush_mode(BrushMode::Mode_Other);
}


/// <summary>
/// Before_renders this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Object::before_render() const noexcept {
    // 纯色笔刷
    if (this->get_brush_mode() == BrushMode::Mode_Color) {
        auto brush = impl::d2d<ID2D1SolidColorBrush>(m_pBrush, IID_ID2D1SolidColorBrush);
        brush->SetColor(impl::d2d(m_color));
    }
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Object::recreate() noexcept -> Result {
    // 重建笔刷资源
    RubyGM::SafeRelease(m_pBrush);
    m_pBrush = Game::GetCommonBrush();
    return Result(S_OK);
}
#endif

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    /// <summary>
    /// Create a <see cref="Font"/> object with the specified prop . never 
    /// </summary>
    /// <param name="">The .</param>
    /// <returns></returns>
    auto Font::Create(const FontProperties& prop) noexcept -> Font& {
        size_t lenp1 = std::wcslen(prop.name) + 1;
        auto ptr = RubyGM::SmallAlloc(sizeof(Font) + lenp1 * sizeof(wchar_t));
        assert(ptr && "RubyGM::SmallAlloc cannot return nullptr");
        auto obj = new(ptr) Asset::Font(prop, lenp1);
        return *obj;
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="Font"/> class.
    /// </summary>
    /// <param name="prop">The property.</param>
    Font::Font(const FontProperties& prop, size_t name_len) noexcept {
        std::memcpy(m_bufFontProp, &prop, sizeof(prop));
        std::memcpy(m_bufFontName, prop.name, name_len * sizeof(prop.name[0]));
    }
    /// <summary>
    /// Finalizes an instance of the <see cref="Font"/> class.
    /// </summary>
    /// <returns></returns>
    Font::~Font() noexcept {
        RubyGM::SafeRelease(m_pTextFormat);
    }
    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    /*auto Font::recreate() noexcept -> Result {
        // 文本格式是CPU资源, 没有有必要重建
        return Result(S_OK);
    }*/
    /// <summary>
    /// Lows the occupancy.
    /// </summary>
    /// <returns></returns>
    void Font::LowOccupancy() noexcept {
        // 在没有被其他对象引用时候释放
        this->LowOccupancyHelper(m_pTextFormat);
    }
    /// <summary>
    /// Releases unmanaged and - optionally - managed resources.
    /// </summary>
    /// <returns></returns>
    void Font::dispose() noexcept {
        // 调用析构函数
        this->Font::~Font();
        // 释放数据
        RubyGM::SmallFree(this);
    }
    /// <summary>
    /// Gets the font.
    /// </summary>
    /// <returns></returns>
    auto Font::GetFont() noexcept ->IGMFont* {
        // 没有则创建
        if (!m_pTextFormat) {
            // 创建
            auto hr = Bridge::CreateFontWithProp(this->prop(), &m_pTextFormat);
            // 检查错误
            if (FAILED(hr)) Game::SetLastErrorCode(hr);
        }
        // 拥有直接返回
        return RubyGM::SafeAcquire(m_pTextFormat);
    }
}}

// stroke
#include <core/asset/rgmAssetStroke.h>

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    /// <summary>
    /// Creates the specified .
    /// </summary>
    /// <param name="">The .</param>
    /// <returns></returns>
    auto Stroke::Create(const StrokeStyle& ss) noexcept -> Stroke& {
        const size_t extra = ss.dash_count * sizeof(float);
        auto ptr = RubyGM::SmallAlloc(sizeof(Stroke) + extra);
        assert(ptr && "RubyGM::SmallAlloc cannot return nullptr");
        auto obj = new(ptr) Asset::Stroke(ss);
        return *obj;
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="Stroke"/> class.
    /// </summary>
    /// <param name="ss">The ss.</param>
    Stroke::Stroke(const StrokeStyle& ss) noexcept : m_style(ss) {
        std::memcpy(m_dashes, ss.dashes, ss.dash_count * sizeof(float));
    }
    /// <summary>
    /// Finalizes an instance of the <see cref=""/> class.
    /// </summary>
    /// <returns></returns>
    Stroke::~Stroke() noexcept {
        RubyGM::SafeRelease(m_pStrokeStyle);
    }
    /// <summary>
    /// Lows the occupancy.
    /// </summary>
    /// <returns></returns>
    void Stroke::dispose() noexcept {
        this->Stroke::~Stroke();
        RubyGM::SmallFree(this);
    }
    /// <summary>
    /// Lows the occupancy.
    /// </summary>
    /// <returns></returns>
    void Stroke::LowOccupancy() noexcept {
        this->LowOccupancyHelper(m_pStrokeStyle);
    }
    /// <summary>
    /// Gets the stroke.
    /// </summary>
    /// <returns></returns>
    auto Stroke::GetStroke() noexcept -> IGMStrokeStyle* {
        // 没有则创建
        if (!m_pStrokeStyle) {
            StrokeStyle ss;
            static_cast<BaseStrokeStyle&>(ss) = m_style;
            ss.dashes = m_dashes;
            // 创建
            auto hr = Bridge::CreateStrokeWithProp(ss, &m_pStrokeStyle);
            // 检查错误
            if (FAILED(hr)) Game::SetLastErrorCode(hr);
        }
        // 拥有直接返回
        return RubyGM::SafeAcquire(m_pStrokeStyle);
    }
}}



/// <summary>
/// Creates the font asset.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Game::CreateFontAsset(
    const FontProperties& fp) noexcept -> RefPtr<Asset::Font> {
    auto&font = Asset::Font::Create(fp); auto* ptr = &font;
    return RefPtr<Asset::Font>(std::move(ptr));
}

/// <summary>
/// Creates the stroke asset.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Game::CreateStrokeAsset(
    const StrokeStyle& ss) noexcept -> RefPtr<Asset::Stroke> {
    auto&stroke = Asset::Stroke::Create(ss); auto* ptr = &stroke;
    return RefPtr<Asset::Stroke>(std::move(ptr));
}