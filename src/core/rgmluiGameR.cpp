#include <bridge/rgmluiGame.h>
#include <bridge/rgmluiBridge.h>
#include <core/asset/rgmAssetFont.h>
#include <core/brush/rgmBrushStruct.h>
#include <core/util/rgmImpl.h>
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
    auto GetNullBitmapAsset() noexcept -> Bitmap& {
        auto& obj = *reinterpret_cast<NullBitmap*>(s_bufNullBitmap);
        obj.AddRef();
        return obj;
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
        this->RegisterFontAsset(rgmfontprop);
    }
    // 添加默认资源 - 笔刷
    {
        RubyGM::NBrushProperties bnprop;
        RubyGM::MakeColorBrush(
            bnprop, 
            impl::rubygm(D2D1::ColorF(D2D1::ColorF::Black))
        );
        this->RegisterBrushAsset(bnprop);
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
/// Gets the bitmap resource, it won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::RefBitmapAsset(
    uint32_t index) noexcept -> Asset::Bitmap & {
    return Bridge::UIGame::GetInstance().RefBitmapAsset(index);
}


/// <summary>
/// Gets the font resource. it won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::RefFontAsset(
    uint32_t index) noexcept -> Asset::Font & {
    return Bridge::UIGame::GetInstance().RefFontAsset(index);
}

/// <summary>
/// Gets the brush resource. it won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::RefBrushAsset(
    uint32_t index) noexcept -> Asset::Brush & {
    return Bridge::UIGame::GetInstance().RefBrushAsset(index);
}

/// <summary>
/// Gets the bitmap resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetBitmapAsset(
    uint32_t index) noexcept -> Asset::Bitmap & {
    auto& asset = Bridge::UIGame::GetInstance().RefBitmapAsset(index);
    asset.AddRef();
    return asset;
}


/// <summary>
/// Gets the font resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetFontAsset(
    uint32_t index) noexcept -> Asset::Font & {
    auto& asset = Bridge::UIGame::GetInstance().RefFontAsset(index);
    asset.AddRef();
    return asset;
}

/// <summary>
/// Gets the brush resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetBrushAsset(uint32_t index) 
noexcept -> Asset::Brush & {
    auto& asset = Bridge::UIGame::GetInstance().RefBrushAsset(index);
    asset.AddRef();
    return asset;
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
auto RubyGM::Game::RegisterBrushAsset
(const NBrushProperties& bnp) noexcept -> uint32_t {
    return Bridge::UIGame::GetInstance().RegisterBrushAsset(bnp);
}

/// <summary>
/// Adds the resource font.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Game::RegisterFontAsset(
    const FontProperties& fp) noexcept -> uint32_t {
    return Bridge::UIGame::GetInstance().RegisterFontAsset(fp);
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
#include <core/brush/rgmBrushStruct.h>
#include <core/util/rgmImpl.h>

/// <summary>
/// Creates the brush with property.
/// </summary>
/// <param name="">The .</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Bridge::CreateBrushWithProp(
    const NBrushProperties& prop, IGMBrush** brush) noexcept -> Result {
    D2D1_BRUSH_PROPERTIES d2d1bprop;
    d2d1bprop.opacity = prop.opacity;
    d2d1bprop.transform = D2D1::Matrix3x2F::Identity();
    switch (prop.type)
    {
    case RubyGM::Type_Color:
        return Result(UIManager_RenderTarget->CreateSolidColorBrush(
            &impl::d2d(prop.color),
            &d2d1bprop,
            reinterpret_cast<ID2D1SolidColorBrush**>(brush)
            ));
    /*case RubyGM::Type_Linear:
        break;
    case RubyGM::Type_Radial:
        break;
    case RubyGM::Type_Other:
        break;*/
    default:
        assert(!"NOIMPL");
        return Result(E_NOTIMPL);
        break;
    }
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
    const FontProperties& fp) noexcept -> uint32_t {
    return this->register_helper<Index_Font>([&fp]() {
        return &Game::CreateFontAsset(fp);
    });
}


/// <summary>
/// Adds the resource brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::RegisterBrushAsset(
    const NBrushProperties& bnp) noexcept -> uint32_t {
    return this->register_helper<Index_Brush>([&bnp]() {
        return &Game::CreateBrushAsset(bnp);
    });
}

