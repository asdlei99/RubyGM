#define _WIN32_WINNT 0x0A000001
#include <core/rubygm.h>
#include <core/graphics/rgmGraphics.h>
#include <core/drawable/effect/rgmEffect.h>
#include <core/drawable/effect/rgmBlur.h>
#include <core/util/rgmImpl.h>
#include <d2d1effects_2.h>

// rubygm namespace
namespace RubyGM {
    // effect graphics interface
    struct IGMEffect : ID2D1Effect {};
    // effect output graphics interface
    struct IGMEffectOutput : ID2D1Image {};
    // bridge
    namespace Bridge {
        // effect from guid
        auto EffectFrom(const GUID& id) noexcept->IGMEffect*;
    }
}

// ============================================================================
// =============================== Effect =====================================
// ============================================================================

/// <summary>
/// Initializes a new instance of the <see cref=""/> .
/// </summary>
/// <param name="">The .</param>
RubyGM::Drawable::Effect::Effect(const EffectStatus& es, const _GUID& id)
noexcept :
Super(es),
m_pEffect(Bridge::EffectFrom(id)),
m_refGuid(id),
des_rect(es.des_rect),
src_rect(es.src_rect){
    if (m_pEffect) {
        m_pEffect->GetOutput(reinterpret_cast<ID2D1Image**>(&m_pOutput));
    }
    if (m_pOutput) {

    }
}


/// <summary>
/// Finalizes an instance of the <see cref=""/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Effect::~Effect() noexcept {
    RubyGM::SafeRelease(m_pEffect);
    RubyGM::SafeRelease(m_pOutput);
}


/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Effect::recreate() noexcept -> Result {
    RubyGM::SafeRelease(m_pEffect);
    RubyGM::SafeRelease(m_pOutput);
    // 现在就需要重建
    if (const auto effect = Bridge::EffectFrom(m_refGuid)) {
        m_pEffect = effect;
        m_pEffect->GetOutput(reinterpret_cast<ID2D1Image**>(&m_pOutput));
    }
    return m_pOutput ? Result(S_OK) : Result(E_FAIL);
}


/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Effect::Render(IGMRenderContext& rc) const noexcept {
    if (!m_pOutput) return;
    const auto w1 = this->src_rect.right - this->src_rect.left;
    const auto h1 = this->src_rect.bottom - this->src_rect.top;
    const auto w2 = this->des_rect.right - this->des_rect.left;
    const auto h2 = this->des_rect.bottom - this->des_rect.top;
    const auto zx = w2 / w1; const auto zy = h2 / h1;
    D2D1_MATRIX_3X2_F transform;  rc.GetTransform(&transform);
    rc.SetTransform(D2D1::Matrix3x2F::Scale({zx, zy}) * transform);
    D2D1_POINT_2F offset{
        des_rect.left / zx, des_rect.top / zy,
    };
    rc.DrawImage(
        m_pOutput, 
        &offset,
        &impl::d2d(src_rect),
        D2D1_INTERPOLATION_MODE_LINEAR,
        D2D1_COMPOSITE_MODE_SOURCE_OVER
    );
    rc.SetTransform(&transform);
}

// ============================================================================
// ============================ GaussianBlur ==================================
// ============================================================================

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    /// <summary>
    /// Initializes a new instance of the <see cref="GaussianBlur"/> class.
    /// </summary>
    /// <param name="">The .</param>
    GaussianBlur::GaussianBlur(const GaussianBlurStatus& gbs) noexcept :
    Super(gbs, CLSID_D2D1GaussianBlur),
        m_fStdDvi(gbs.sd),
        m_refBitmapAsset(gbs.bitmap) {
        // 初始化
        m_refBitmapAsset.AddRef();
        this->init();
    }
    /// <summary>
    /// Finalizes an instance of the <see cref="GaussianBlur"/> class.
    /// </summary>
    /// <returns></returns>
    GaussianBlur::~GaussianBlur() noexcept {
        m_refBitmapAsset.Release();
    }
    /// <summary>
    /// Initializes this instance.
    /// </summary>
    /// <returns></returns>
    void GaussianBlur::init() noexcept {
        // 创建成功
        if (this->is_ok()) {
            // 做为输入
            if (auto bmp = m_refBitmapAsset.GetBitmap()) {
                this->single_input_init(bmp);
                bmp->Release();
            }
            // 写入
            auto old = m_fStdDvi; m_fStdDvi = -1.f;
            this->SetStandardDeviation(old);
            // 为速度优化
            constexpr auto index = D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION;
            constexpr auto value = D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED;
            m_pEffect->SetValue(index, value);
        }
    }
    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    auto GaussianBlur::recreate() noexcept -> Result {
        Result hr = S_OK;
        // 重建父类
        if (SUCCEEDED(hr)) {
            hr = Super::recreate();
        }
        // 重建资源
        if (SUCCEEDED(hr)) {
            hr = m_refBitmapAsset.Recreate();
        }
        // 设置资源
        if (SUCCEEDED(hr)) {
            this->init();
        }
        // 扫尾
        return hr;
    }
    /// <summary>
    /// Gets the standard deviation.
    /// </summary>
    /// <param name="sd">The sd.</param>
    /// <returns></returns>
    auto GaussianBlur::SetStandardDeviation(float sd) noexcept ->Result {
        assert(this->is_ok() && "are you ok?");
        if (!this->is_ok()) return Result(E_NOT_SET);
        if (m_fStdDvi == sd) return Result(S_FALSE); m_fStdDvi = sd;
        constexpr auto index = D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION;
        return Result(m_pEffect->SetValue(index, sd));
    }
    /// <summary>
    /// Creates the specified GBS.
    /// </summary>
    /// <param name="gbs">The GBS.</param>
    /// <returns></returns>
    auto GaussianBlur::Create(const GaussianBlurStatus& gbs)
    noexcept -> GaussianBlur * {
        // 申请空间
        if (auto ptr = RubyGM::SmallAlloc(sizeof(GaussianBlur))) {
            // 创建成功
            auto obj = new(ptr) GaussianBlur(gbs);
            // 状态OK
            if (obj->is_ok()) return obj;
            //否则释放掉
            obj->dispose();
        }
        return nullptr;
    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void GaussianBlur::dispose() noexcept {
        this->GaussianBlur::~GaussianBlur();
        RubyGM::SmallFree(this);
    }
}}


// ============================================================================
// ========================== DirectionalBlur =================================
// ============================================================================

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {

}}

// ============================================================================
// =============================== Shadow =====================================
// ============================================================================
#include <core/drawable/effect/rgmShadow.h>

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    /// <summary>
    /// Initializes a new instance of the <see cref="Shadow"/> class.
    /// </summary>
    /// <param name="">The .</param>
    Shadow::Shadow(const ShadowStatus& gbs) noexcept :
    Super(gbs, CLSID_D2D1Shadow),
        m_fStdDvi(gbs.blur_sd),
        m_color(gbs.color),
        m_refBitmapAsset(gbs.bitmap) {
        // 初始化
        m_refBitmapAsset.AddRef();
        this->init();
    }
    /// <summary>
    /// Finalizes an instance of the <see cref="Shadow"/> class.
    /// </summary>
    /// <returns></returns>
    Shadow::~Shadow() noexcept {
        m_refBitmapAsset.Release();
    }
    /// <summary>
    /// Initializes this instance.
    /// </summary>
    /// <returns></returns>
    void Shadow::init() noexcept {
        // 创建成功
        if (this->is_ok()) {
            // 做为输入
            if (auto bmp = m_refBitmapAsset.GetBitmap()) {
                this->single_input_init(bmp);
                bmp->Release();
            }
            // 写入
            auto old = m_fStdDvi; m_fStdDvi = -1.f;
            this->SetStandardDeviation(old);
            auto color = m_color;
            this->SetColor(color);
            // 为速度优化
            constexpr auto index = D2D1_SHADOW_PROP_OPTIMIZATION;
            constexpr auto value = D2D1_SHADOW_OPTIMIZATION_SPEED;
            m_pEffect->SetValue(index, value);
        }
    }
    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    auto Shadow::recreate() noexcept -> Result {
        Result hr = S_OK;
        // 重建父类
        if (SUCCEEDED(hr)) {
            hr = Super::recreate();
        }
        // 重建资源
        if (SUCCEEDED(hr)) {
            hr = m_refBitmapAsset.Recreate();
        }
        // 设置资源
        if (SUCCEEDED(hr)) {
            this->init();
        }
        // 扫尾
        return hr;
    }
    /// <summary>
    /// Gets the standard deviation.
    /// </summary>
    /// <param name="sd">The sd.</param>
    /// <returns></returns>
    auto Shadow::SetStandardDeviation(float sd) noexcept ->Result {
        assert(this->is_ok() && "are you ok?");
        if (!this->is_ok()) return Result(E_NOT_SET);
        if (m_fStdDvi == sd) return Result(S_FALSE); m_fStdDvi = sd;
        constexpr auto index = D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION;
        return Result(m_pEffect->SetValue(index, sd));
    }
    /// <summary>
    /// Sets the color.
    /// </summary>
    /// <param name="color">The color.</param>
    /// <returns></returns>
    auto Shadow::SetColor(const ColorF& color) noexcept -> Result {
        assert(this->is_ok() && "are you ok?");
        if (!this->is_ok()) return Result(E_NOT_SET);
        m_color = color;
        constexpr auto index = D2D1_SHADOW_PROP_COLOR;
        return Result(m_pEffect->SetValue(index, m_color));
    }
    /// <summary>
    /// Creates the specified GBS.
    /// </summary>
    /// <param name="gbs">The GBS.</param>
    /// <returns></returns>
    auto Shadow::Create(const ShadowStatus& gbs) noexcept -> Shadow* {
        // 申请空间
        if (auto ptr = RubyGM::SmallAlloc(sizeof(Shadow))) {
            // 创建成功
            auto obj = new(ptr) Shadow(gbs);
            // 状态OK
            if (obj->is_ok()) return obj;
            //否则释放掉
            obj->dispose();
        }
        return nullptr;
    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void Shadow::dispose() noexcept {
        this->Shadow::~Shadow();
        RubyGM::SmallFree(this);
    }
}}

