#include <bridge/rgmluiGame.h>
#include <bridge/rgmluiBridge.h>
#include <core/asset/rgmAssetFont.h>
#include <core/util/rgmBrushStruct.h>
#include <core/graphics/rgmGraphics.h>
#include <core/util/rgmImpl.h>
#include <core/util/rgmUtil.h>
#include <algorithm>
#undef min
#undef max

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    // null bitmap, return this object if OOM
    class NullBitmap final : public Bitmap {
    public:
        // get bitmap
        auto GetBitmap() noexcept -> IGMBitmap* override { return nullptr; }
        // low occu
        void LowOccupancy() noexcept override {}
    public:
        // ctor
        NullBitmap() noexcept {}
        // dtor
        ~NullBitmap() noexcept {}
    protected:
        // recreate
        //auto recreate() noexcept->uint32_t override { return 0; }
    private:
        // dispose
        void dispose() noexcept override {}
    };
    // bufer
    size_t s_bufNullBitmap[RubyGM::AlignedSizeTLen(sizeof(NullBitmap))];
    // get null bitmap object
    auto GetNullBitmapAsset() noexcept ->RefPtr<Asset::Bitmap> {
        auto ptr = reinterpret_cast<NullBitmap*>(s_bufNullBitmap);
        auto obj = static_cast<Asset::Bitmap*>(ptr);
        return RefPtr<Asset::Bitmap>(obj);
    }
    // get null bitmap object - ex
    static inline auto RefNullBitmapAssetEx() noexcept -> NullBitmap& {
        return *reinterpret_cast<NullBitmap*>(s_bufNullBitmap);
    }
}}

/// <summary>
/// Initializes the resource.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::init_resource() noexcept {
    //assert(!"NOIMPL");
    // 创建
    Asset::RefNullBitmapAssetEx().Asset::NullBitmap::NullBitmap();
    // 添加默认资源 - 字体
    {
        constexpr size_t len1 = sizeof(RubyGM::FontProperties);
        constexpr size_t len2 = sizeof(wchar_t) * MAX_PATH;
        alignas(void*) char buffer[len1 + len2];
        using tfp = LongUI::DX::TextFormatProperties*;
        auto& luidxtfprop = *reinterpret_cast<tfp>(buffer);
        auto& rgmfontprop = reinterpret_cast<FontProperties&>(luidxtfprop);
        LongUI::DX::InitTextFormatProperties(luidxtfprop, MAX_PATH);
        auto font = Game::CreateFontAsset(rgmfontprop);
        this->RegisterFontAsset(*font);
    }
    // 添加默认资源 - 笔刷
    {
        ColorF color; color.r = color.g = color.b = 0.f; color.a = 1.f;
        auto brush = Game::CreateBrushAsset(color);
        this->RegisterBrushAsset(*brush);
    }
}


/// <summary>
/// Release_assets this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::release_asset() noexcept {
    // 释放所有资源
    for (size_t i = 0; i < RESOURCE_COUNT; ++i) {
        assert(m_appAssetPtr[i] && "bad action");
        auto begin = m_appAssetPtr[i];
        auto end = m_appAssetPtr[i] + m_acAssetPtr[i];
        // 释放每部分资源
        std::for_each(begin, end, [](Asset::Object* obj) {
            obj->Release();
        });
    }
    // 释放
    Asset::RefNullBitmapAssetEx().Asset::NullBitmap::~NullBitmap();
}


/// <summary>
/// Adds the sprite.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::AddSprite(
    const SprteStatus& ss) noexcept -> CGMSprite* {
    return m_sprRoot.AddChild(ss);
}


/// <summary>
/// News the sprite.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Game::NewSprite(const SprteStatus& ss) noexcept -> CGMSprite* {
    return RubyGM::Bridge::UIGame::GetInstance().AddSprite(ss);
}



/// <summary>
/// Gets the last error code.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetLastErrorCode() noexcept->Result {
    return RubyGM::Bridge::UIGame::GetInstance().error_code;
}

/// <summary>
/// Gets the last resource object.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::GetLastResourceObject() noexcept -> Base::Resource* {
    return RubyGM::Bridge::UIGame::GetInstance().GetResourceTail();
}

/// <summary>
/// Gets the color brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::GetCommonBrush() noexcept ->IGMBrush* {
    auto brush = RubyGM::Bridge::UIGame::GetInstance().GetCommonBrush();
    return reinterpret_cast<IGMBrush*>(brush);
}

/// <summary>
/// Creates the batch.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::CreateBatch() noexcept -> IGMBatch* {
    ID2D1SpriteBatch* batch = nullptr;
    auto hr = UIManager_RenderTarget->CreateSpriteBatch(&batch);
    if (FAILED(hr)) Game::SetLastErrorCode(Result(hr));
    return static_cast<IGMBatch*>(batch);
}



/// <summary>
/// Gets the time scale.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetTimeScale() noexcept -> float {
    return RubyGM::Bridge::UIGame::GetInstance().time_scale;
}

/// <summary>
/// Sets the time scale.
/// </summary>
/// <param name="ts">The ts.</param>
/// <returns></returns>
void RubyGM::Game::SetTimeScale(float ts) noexcept {
    ts = std::max(std::min(ts, 100.f), 0.f);
    RubyGM::Bridge::UIGame::GetInstance().time_scale = ts;
}

// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // get now time
    auto get_time() noexcept -> uint32_t { return ::timeGetTime(); }
    // get root transform
    auto get_root_transform() noexcept -> const Matrix3X2F& {
        return Bridge::UIGame::GetInstance().GetRootTransform();
    };
}}


/// <summary>
/// Gets the bitmap resource..but won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RefBitmapAsset(uint32_t index) 
const noexcept -> Asset::Bitmap& {
    // 检查范围
    if (uint16_t(index) >= m_acAssetPtr[Index_Bitmap]) {
        index = 0;
#ifdef _DEBUG
        UIManager << DL_Warning
            << L"index out of range, now set to 0"
            << LongUI::endl;
#endif
    }
    // 返回对象引用
    auto& asset = *m_appAssetPtr[Index_Bitmap][index];
    return static_cast<Asset::Bitmap&>(asset);
}

/// <summary>
/// Gets the font resource..but won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RefFontAsset(uint32_t index) 
const noexcept -> Asset::Font& {
    // 检查范围
    if (uint16_t(index) >= m_acAssetPtr[Index_Font]) {
        index = 0;
#ifdef _DEBUG
        UIManager << DL_Warning
            << L"index out of range, now set to 0"
            << LongUI::endl;
#endif
    }
    // 返回对象引用
    auto& asset = *m_appAssetPtr[Index_Font][index];
    return static_cast<Asset::Font&>(asset);
}

/// <summary>
/// Gets the brush resource.but won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RefBrushAsset(uint32_t index) 
const noexcept -> Asset::Brush& {
    // 检查范围
    if (uint16_t(index) >= m_acAssetPtr[Index_Brush]) {
        index = 0;
#ifdef _DEBUG
        UIManager << DL_Warning
            << L"index out of range, now set to 0"
            << LongUI::endl;
#endif
    }
    // 返回引用
    auto& asset = *m_appAssetPtr[Index_Brush][index];
    return static_cast<Asset::Brush&>(asset);
}

/// <summary>
/// Gets the bitmap resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetBitmapAsset(
    uint32_t index) noexcept -> RefPtr<Asset::Bitmap> {
    auto& asset = Bridge::UIGame::GetInstance().RefBitmapAsset(index);
    return RefPtr<Asset::Bitmap>(&asset);
}


/// <summary>
/// Gets the font resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetFontAsset(
    uint32_t index) noexcept -> RefPtr<Asset::Font> {
    auto& asset = Bridge::UIGame::GetInstance().RefFontAsset(index);
    return RefPtr<Asset::Font>(&asset);
}

/// <summary>
/// Gets the brush resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetBrushAsset(
    uint32_t index) noexcept -> RefPtr<Asset::Brush> {
    auto& asset = Bridge::UIGame::GetInstance().RefBrushAsset(index);
    return RefPtr<Asset::Brush>(&asset);
}




/// <summary>
/// Adds the resource bitmap.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::RegisterBitmapAsset() noexcept -> uint32_t {
    return Bridge::UIGame::GetInstance().RegisterBitmapAsset();
}


/// <summary>
/// Adds the resource brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::RegisterBrushAsset(
    Asset::Brush& b) noexcept -> uint32_t {
    return Bridge::UIGame::GetInstance().RegisterBrushAsset(b);
}

/// <summary>
/// Registers the font asset.
/// </summary>
/// <param name="brush">The brush.</param>
/// <returns></returns>
auto RubyGM::Game::RegisterFontAsset(
    Asset::Font& f) noexcept -> uint32_t {
    return Bridge::UIGame::GetInstance().RegisterFontAsset(f);
}

/// <summary>
/// Creates the font with property.
/// </summary>
/// <param name="">The .</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateFontWithProp(
    const FontProperties& fp, IGMFont** font) noexcept -> Result {
    auto& prop = reinterpret_cast<const LongUI::DX::TextFormatProperties&>(fp);
    auto* tfmt = reinterpret_cast<IDWriteTextFormat**>(font);
    auto hr = LongUI::DX::CreateTextFormat(prop, tfmt);
    assert(SUCCEEDED(hr));
    return Result(hr);
}


// brush
#include <core/util/rgmBrushStruct.h>
#include <core/util/rgmImpl.h>

// rubygm::impl naemspace
namespace RubyGM { namespace impl {
    // GradientStop
    auto d2d(const GradientStop* s) noexcept {
        constexpr auto a = sizeof(GradientStop);
        constexpr auto b = sizeof(D2D1_GRADIENT_STOP);
        static_assert(a == b, "bad cast");
        constexpr auto c = offsetof(GradientStop, color);
        constexpr auto d = offsetof(D2D1_GRADIENT_STOP, color);;
        static_assert(c == d, "bad cast");
        return reinterpret_cast<const D2D1_GRADIENT_STOP*>(s);
    }
}}


/// <summary>
/// Creates the stroke with property.
/// </summary>
/// <param name="fp">The fp.</param>
/// <param name="stroke">The stroke.</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateStrokeWithProp(
    const StrokeStyle& ss, IGMStrokeStyle** stroke) noexcept -> Result {
    D2D1_STROKE_STYLE_PROPERTIES ssp;
    ssp.startCap    = D2D1_CAP_STYLE(ss.cap_begin);
    ssp.endCap      = D2D1_CAP_STYLE(ss.cap_end);
    ssp.dashCap     = D2D1_CAP_STYLE(ss.cap_dash);
    ssp.lineJoin    = D2D1_LINE_JOIN(ss.line_join);
    ssp.miterLimit  = ss.miter_limit;
    ssp.dashStyle   = D2D1_DASH_STYLE_CUSTOM;
    ssp.dashOffset  = ss.dash_offset;
    return Result(UIManager_D2DFactory->CreateStrokeStyle(
        &ssp,
        ss.dashes,
        ss.dash_count, 
        reinterpret_cast<ID2D1StrokeStyle**>(stroke)
    ));
}

/// <summary>
/// Creates the color-brush with property.
/// </summary>
/// <param name="color">The color.</param>
/// <param name="brush">The brush.</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateBrushWithProp(
    const ColorF& color, IGMBrush** brush) noexcept -> Result {
    return Result(UIManager_RenderTarget->CreateSolidColorBrush(
        &impl::d2d(color),
        nullptr,
        reinterpret_cast<ID2D1SolidColorBrush**>(brush)
    ));
}


/// <summary>
/// Creates the brush with property.
/// </summary>
/// <param name="bitmap">The bitmap.</param>
/// <param name="brush">The brush.</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateBrushWithProp(
    IGMBitmap* bitmap, IGMBrush ** brush) noexcept -> Result {
    assert(bitmap && brush && "bad argment");
    D2D1_BITMAP_BRUSH_PROPERTIES1 bbp;
    bbp.extendModeX = D2D1_EXTEND_MODE_MIRROR;
    bbp.extendModeY = D2D1_EXTEND_MODE_MIRROR;
    bbp.interpolationMode = D2D1_INTERPOLATION_MODE_LINEAR;
    return Result(UIManager_RenderTarget->CreateBitmapBrush(
        reinterpret_cast<ID2D1Bitmap1*>(bitmap), 
        &bbp,
        nullptr,
        reinterpret_cast<ID2D1BitmapBrush1**>(brush)
    ));
}


/// <summary>
/// Creates the linear-gradient-brush with property.
/// </summary>
/// <param name="linear">The linear.</param>
/// <param name="brush">The brush.</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateBrushWithProp(
    const LinearBrush& linear, IGMBrush** brush) noexcept -> Result {
    auto hr = S_OK;
    ID2D1GradientStopCollection* collection = nullptr;
    // 创建渐变色集
    if (SUCCEEDED(hr)) {
        hr = UIManager_RenderTarget->CreateGradientStopCollection(
            impl::d2d(linear.stops),
            linear.count,
            &collection
        );
    }
    // 创建线性渐变笔刷
    if (SUCCEEDED(hr)) {
        D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES gbp;
        gbp.startPoint = impl::d2d(linear.begin);
        gbp.endPoint = impl::d2d(linear.end);
        hr = UIManager_RenderTarget->CreateLinearGradientBrush(
            &gbp, 
            nullptr, 
            collection, 
            reinterpret_cast<ID2D1LinearGradientBrush**>(brush)
        );
    }
    // 扫尾处理
    RubyGM::SafeRelease(collection);
    // 返回结果
    return Result(hr);
}

/// <summary>
/// Creates the radial-gradien-brush with property.
/// </summary>
/// <param name="radial">The radial.</param>
/// <param name="brush">The brush.</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateBrushWithProp(
    const RadialBrush& radial, IGMBrush** brush) noexcept -> Result {
    auto hr = S_OK;
    ID2D1GradientStopCollection* collection = nullptr;
    // 创建渐变色集
    if (SUCCEEDED(hr)) {
        hr = UIManager_RenderTarget->CreateGradientStopCollection(
            impl::d2d(radial.stops),
            radial.count,
            &collection
        );
    }
    // 创建线性渐变笔刷
    if (SUCCEEDED(hr)) {
        D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES rbp;
        rbp.center = impl::d2d(radial.center);
        rbp.gradientOriginOffset = impl::d2d(radial.offset);
        rbp.radiusX = radial.rx;
        rbp.radiusY = radial.ry;
        hr = UIManager_RenderTarget->CreateRadialGradientBrush(
            &rbp, 
            nullptr, 
            collection, 
            reinterpret_cast<ID2D1RadialGradientBrush**>(brush)
        );
    }
    // 扫尾处理
    RubyGM::SafeRelease(collection);
    // 返回结果
    return Result(hr);
}




/// <summary>
/// Adds the resource bitmap.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RegisterBitmapAsset(
) noexcept -> uint32_t {
    return m_acAssetPtr[Index_Bitmap];
}

/// <summary>
/// Adds the resource font.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RegisterFontAsset(
    Asset::Font& font) noexcept -> uint32_t {
    return this->register_helper<Index_Font>([&font]() {
        auto ptr = &font; assert(ptr && "cannot be null");
        return ptr;
    });
}


/// <summary>
/// Adds the resource brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RegisterBrushAsset(
    Asset::Brush& brush) noexcept -> uint32_t {
    return this->register_helper<Index_Brush>([&brush]() {
        auto ptr = &brush; assert(ptr && "cannot be null");
        return ptr;
    });
}

