#define _WIN32_WINNT 0x0A000001
#include <core/rubygm.h>
#include <core/graphics/rgmGraphics.h>
#include <core/Drawable/rgmBitmap.h>
#include <core/util/rgmImpl.h>
#include <game/rgmGame.h>
#include <algorithm>
#undef min

/// <summary>
/// Initializes a new instance of the <see cref="Bitmap"/> class.
/// </summary>
/// <param name="bs">The bs.</param>
RubyGM::Drawable::Bitmap::Bitmap(const BitmapStatus& bs) noexcept :
Super(bs),
m_spAsBitmap(bs.bitmap),
src_rect(bs.src),
des_rect(bs.des),
m_modeInter(Mode_Linear),
m_pGiBitmap(bs.bitmap->GetBitmap()) {
    this->reset_bitmap_size();
}

/// <summary>
/// Sets the interpolation mode.
/// </summary>
/// <param name="mode">The mode.</param>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::SetInterpolationMode(
    InterpolationMode mode) noexcept {
    m_modeInter = mode > Mode_HighQqualityCubic ?
        Mode_Linear : mode;
}

/// <summary>
/// reset the size of bitmap
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::reset_bitmap_size() noexcept {
    // 修改大小
    if (m_pGiBitmap) {
        auto size = m_pGiBitmap->GetSize();
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
    RubyGM::SafeRelease(m_pGiBitmap);
}


/// <summary>
/// Finalizes an instance of the <see cref="Bitmap"/> class.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::dispose() noexcept {
    this->Bitmap::~Bitmap();
    RubyGM::SmallFree(this);
}


/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Bitmap::Render(IGMRenderContext& rc) const noexcept {
    if (!m_pGiBitmap) return;
#ifdef _DEBUG
    assert((m_pGiBitmap->GetOptions() & D2D1_BITMAP_OPTIONS_CANNOT_DRAW) == 0);
#endif
    auto mode = D2D1_INTERPOLATION_MODE(m_modeInter);
    auto& des = impl::d2d(this->des_rect);
    auto& src = impl::d2d(this->src_rect);
    rc.DrawBitmap(m_pGiBitmap, &des, this->opacity, mode, &src, nullptr);
}


/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Bitmap::recreate() noexcept -> Result {
    RubyGM::SafeRelease(m_pGiBitmap);
    uint32_t hr = Result(S_OK);
    // 重建父类
    if (SUCCEEDED(hr)) {
        hr = Super::recreate();
    }
    // 重建资源
    if (SUCCEEDED(hr)) {
        hr = m_spAsBitmap->Recreate();
    }
    // 重获位图
    if (SUCCEEDED(hr)) {
        m_pGiBitmap = m_spAsBitmap->GetBitmap();
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
    assert(bs.bitmap && "bitmap from BitmapStatus cannot be null");
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Bitmap))) {
        return new(ptr) Bitmap(bs);
    }
    return nullptr;
}

/// <summary>
/// Initializes a new instance of the <see cref="PerspectiveBitmap"/> class.
/// </summary>
/// <param name="bs">The bs.</param>
RubyGM::Drawable::PerspectiveBitmap::PerspectiveBitmap(
    const PerspectiveBitmapStatus& bs) noexcept : Super(bs) {
    auto&pers = reinterpret_cast<D2D1::Matrix4x4F&>(this->perspective);
    pers.D2D1::Matrix4x4F::Matrix4x4F();
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::PerspectiveBitmap::dispose() noexcept {
    this->PerspectiveBitmap::~PerspectiveBitmap();
    RubyGM::NormalFree(this);
}


/// <summary>
/// Creates the specified bs.
/// </summary>
/// <param name="bs">The bs.</param>
/// <returns></returns>
auto RubyGM::Drawable::PerspectiveBitmap::Create(
    const PerspectiveBitmapStatus& bs) noexcept -> PerspectiveBitmap* {
    assert(bs.bitmap && "bitmap from BitmapStatus cannot be null");
    // 申请空间
    if (const auto ptr = RubyGM::NormalAlloc(sizeof(PerspectiveBitmap))) {
        return new(ptr) PerspectiveBitmap(bs);
    }
    return nullptr;
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::PerspectiveBitmap::Render(
    IGMRenderContext& rc) const noexcept {
    if (!m_pGiBitmap) return;
#ifdef _DEBUG
    assert((m_pGiBitmap->GetOptions() & D2D1_BITMAP_OPTIONS_CANNOT_DRAW) == 0);
#endif
    auto mode = D2D1_INTERPOLATION_MODE(m_modeInter);
    auto& des = impl::d2d(this->des_rect);
    auto& src = impl::d2d(this->src_rect);
    auto& per = impl::d2d(this->perspective);
    rc.DrawBitmap(m_pGiBitmap, &des, this->opacity, mode, &src, &per);
}


// ============================================================================
// ================================= Mask =====================================
// ============================================================================
#include <core/drawable/rgmMask.h>


/// <summary>
/// Initializes a new instance of the <see cref="Mask"/> class.
/// </summary>
/// <param name="ms">The ms.</param>
RubyGM::Drawable::Mask::Mask(const MaskStatus& ms) noexcept :
Super(ms), m_spAsMask(ms.mask), m_spAsBrush(ms.brush),
m_pGiMask(ms.mask->GetBitmap()),m_pGiBrush(ms.brush->GetBrush()) {

}

/// <summary>
/// Creates the specified bs.
/// </summary>
/// <param name="bs">The bs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Mask::Create(const MaskStatus& ms) noexcept ->Mask*{
    assert(ms.mask && "bitmap from MaskStatus cannot be null");
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Mask))) {
        return new(ptr) Mask(ms);
    }
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Mask"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Mask::~Mask() noexcept {
    RubyGM::SafeRelease(m_pGiMask);
    RubyGM::SafeRelease(m_pGiBrush);
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Mask::dispose() noexcept {
    this->Mask::~Mask();
    RubyGM::SmallFree(this);
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Mask::recreate() noexcept -> Result {
    RubyGM::SafeRelease(m_pGiMask);
    RubyGM::SafeRelease(m_pGiBrush);
    uint32_t hr = Result(S_OK);
    // 重建父类
    if (SUCCEEDED(hr)) {
        hr = Super::recreate();
    }
    // 重建遮罩
    if (SUCCEEDED(hr)) {
        hr = m_spAsMask->Recreate();
    }
    // 重建笔刷
    if (SUCCEEDED(hr)) {
        hr = m_spAsMask->Recreate();
    }
    // 重获资源
    if (SUCCEEDED(hr)) {
        m_pGiMask = m_spAsMask->GetBitmap();
        m_pGiBrush = m_spAsBrush->GetBrush();
#ifdef _DEBUG
        assert(m_pGiBrush && m_pGiMask && "error check");
#endif
    }
    return hr;
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Mask::Render(IGMRenderContext& rc) const noexcept {
    if (m_pGiMask && m_pGiBrush) {
        auto mode = rc.GetAntialiasMode();
        rc.SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        rc.FillOpacityMask(m_pGiMask, m_pGiBrush);
        rc.SetAntialiasMode(mode);
    }
}



// ============================================================================
// ================================ Batch =====================================
// ============================================================================

#include <core/drawable/rgmBatch.h>
#include <bridge/rgmluiBridge.h>


/// <summary>
/// Updates the batch.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Batch::UpdateBatch() noexcept {
    if (m_pGiBatch && m_pGiBitmap) {
        auto& this_ = static_cast<XGMBatchController&>(*this);
        auto& other = *m_pController;
        other.UpdateBatch(*m_pGiBatch, *m_pGiBitmap);
        this_ = other;
    }
}

/// <summary>
/// reset batches
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Batch::reset_batch() noexcept {
    this->start_index = 0;
    this->display_count = 0;
    this->unit_count = 0;
    if (m_pGiBatch && m_pGiBitmap) {
        auto& this_ = static_cast<XGMBatchController&>(*this);
        auto& other = *m_pController;
        other.ResetBatch(*m_pGiBatch, *m_pGiBitmap);
        this_ = other;
    }
}

/// <summary>
/// Resets the batch.
/// </summary>
/// <param name="a">a.</param>
/// <param name="b">The b.</param>
/// <returns></returns>
void RubyGM::Drawable::Batch::ResetBatch(IGMBatch& a, IGMBitmap& b) noexcept {
    auto size = b.GetPixelSize();
    D2D1_RECT_U src{ 0, 0, size.width, size.height };
    D2D1_RECT_F des{ 0.f, 0.f, float(size.width), float(size.height) };
    auto hr = a.AddSprites(
        1, &des, &src, nullptr, nullptr, 0, 0, 0, 0
    );
    assert(SUCCEEDED(hr));
    if (SUCCEEDED(hr)) {
        this->start_index = 0;
        this->display_count = 1;
        this->unit_count = 1;
    }
}

/// <summary>
/// Updates the batch.
/// </summary>
/// <param name="">The .</param>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::Drawable::Batch::UpdateBatch(IGMBatch&, IGMBitmap&) noexcept {

}


/// <summary>
/// Initializes a new instance of the <see cref="Mask"/> class.
/// </summary>
/// <param name="ms">The ms.</param>
RubyGM::Drawable::Batch::Batch(const BatchStatus& bs) noexcept : Super(bs), 
m_spAsBitmap(bs.bitmap), 
m_pGiBitmap(bs.bitmap->GetBitmap()),
m_pGiBatch(Bridge::CreateBatch()), 
m_pController(bs.controller ? bs.controller : this) {
    this->reset_batch();
}

/// <summary>
/// Creates the specified bs.
/// </summary>
/// <param name="bs">The bs.</param>
/// <returns></returns>
auto RubyGM::Drawable::Batch::Create(const BatchStatus& bs) noexcept ->Batch* {
    assert(bs.bitmap && "bitmap from BatchStatus cannot be null");
    // 申请空间
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Batch))) {
        return new(ptr) Batch(bs);
    }
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Mask"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Batch::~Batch() noexcept {
    RubyGM::SafeRelease(m_pGiBitmap);
    RubyGM::SafeRelease(m_pGiBatch);
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Batch::dispose() noexcept {
    this->Batch::~Batch();
    RubyGM::SmallFree(this);
}


/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Batch::recreate() noexcept -> Result {
    RubyGM::SafeRelease(m_pGiBitmap);
    RubyGM::SafeRelease(m_pGiBatch);
    Result hr = Result(S_OK);
    // 重建位图
    if (SUCCEEDED(hr)) {
        hr = m_spAsBitmap->Recreate();
    }
    // 重获位图, 精灵集
    if (SUCCEEDED(hr)) {
        m_pGiBitmap = m_spAsBitmap->GetBitmap();
        m_pGiBatch = Bridge::CreateBatch();
        this->reset_batch();
    }
    return hr;
}


/// <summary>
/// Disconnects the controller.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Batch::DisconnectController() noexcept {
    m_pController = this;
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Batch::Render(IGMRenderContext& rc) const noexcept {
    if (m_pGiBatch && m_pGiBitmap) {
        // 过多的精灵单元
        if (this->display_count > 256) {
            auto hr = rc.Flush();
            if (FAILED(hr)) Game::SetLastErrorCode(hr);
        }
        auto antimode = rc.GetAntialiasMode();
        rc.SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
        rc.DrawSpriteBatch(
            m_pGiBatch,
            this->start_index,
            this->display_count,
            m_pGiBitmap,
            D2D1_BITMAP_INTERPOLATION_MODE(this->mode),
            D2D1_SPRITE_OPTIONS_NONE
        );
        rc.SetAntialiasMode(antimode);
    }
}
