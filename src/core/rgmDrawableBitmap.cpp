#include <core/rubygm.h>
#include <core/graphics/rgmGraphics.h>
#include <core/Drawable/rgmBitmap.h>
#include <core/util/rgmImpl.h>
#include <algorithm>
#undef min

/// <summary>
/// Initializes a new instance of the <see cref="Bitmap"/> class.
/// </summary>
/// <param name="bs">The bs.</param>
RubyGM::Drawable::Bitmap::Bitmap(const BitmapStatus& bs) noexcept :
Super(bs),
m_refBitmap(bs.bitmap),
src_rect(bs.src),
des_rect(bs.des),
m_modeInter(Mode_Linear),
m_pBitmap(bs.bitmap.GetBitmap()) {
    m_refBitmap.AddRef();
    this->reset_bitmap_size();
}

/// <summary>
/// Sets the interpolation mode.
/// </summary>
/// <param name="mode">The mode.</param>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::SetInterpolationMode(
    InterpolationMode mode) noexcept {
    m_modeInter = std::min(Mode_HighQqualityCubic, mode);
}


/// <summary>
/// Saves as PNG.
/// </summary>
/// <param name="file_name">The file_name.</param>
/// <returns></returns>
auto RubyGM::Drawable::Bitmap::SaveAsPng(
    const wchar_t* file_name) noexcept ->Result {
    return m_refBitmap.SaveAsPng(file_name);
}

/// <summary>
/// Saves as PNG.
/// </summary>
/// <param name="file_name">The file_name.</param>
/// <returns></returns>
auto RubyGM::Drawable::Bitmap::SaveAsPng(
    const char* file_name) noexcept ->Result {
    return m_refBitmap.SaveAsPng(file_name);
}



/// <summary>
/// reset the size of bitmap
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::reset_bitmap_size() noexcept {
    // 修改大小
    if (m_pBitmap) {
        auto size = m_pBitmap->GetSize();
        m_fWidth = size.width;
        m_fHeight = size.height;
    }
    else {
        m_fWidth = 1.f;
        m_fHeight = 1.f;
    }
    // 修改源矩形
    if (this->src_rect.right == 0.f) {
        this->src_rect = {
            0.f, 0.f, m_fWidth, m_fHeight
        };
    }
    // 修改目标矩形
    if (this->des_rect.right == 0.f) {
        this->des_rect = {
            0.f, 0.f, m_fWidth, m_fHeight
        };
    }
}

/// <summary>
/// Finalizes an instance of the <see cref="Bitmap"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Bitmap::~Bitmap() noexcept {
    RubyGM::SafeRelease(m_pBitmap);
    m_refBitmap.Release();
}


/// <summary>
/// Finalizes an instance of the <see cref="Bitmap"/> class.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::dispose() noexcept {
    this->Bitmap::~Bitmap();
    RubyGM::NormalFree(this);
}


/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::Render(IGMRenderContext& rc) const noexcept {
    if (!m_pBitmap) return;
#ifdef _DEBUG
    assert((m_pBitmap->GetOptions() & D2D1_BITMAP_OPTIONS_CANNOT_DRAW) == 0);
#endif
    auto mode = D2D1_INTERPOLATION_MODE(m_modeInter);
    auto& des = impl::d2d(this->des_rect);
    auto& src = impl::d2d(this->src_rect);
    D2D1_MATRIX_4X4_F* perspective = nullptr;
    rc.DrawBitmap(m_pBitmap, &des, this->opacity, mode, &src, perspective);
}


/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Bitmap::recreate() noexcept -> Result {
    RubyGM::SafeRelease(m_pBitmap);
    uint32_t hr = Result(S_OK);
    // 重建父类
    if (SUCCEEDED(hr)) {
        hr = Super::recreate();
    }
    // 重建资源
    if (SUCCEEDED(hr)) {
        hr = m_refBitmap.Recreate();
    }
    // 重建资源
    if (SUCCEEDED(hr)) {
        m_pBitmap = m_refBitmap.GetBitmap();
        this->reset_bitmap_size();
    }
    return hr;
}

/// <summary>
/// Creates the specified bs.
/// </summary>
/// <param name="bs">The bs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Bitmap::Create(
    const BitmapStatus& bs) noexcept -> Bitmap* {
    // 申请空间
    if (const auto ptr = RubyGM::NormalAlloc(sizeof(Bitmap))) {
        return new(ptr) Bitmap(bs);
    }
    return nullptr;
}