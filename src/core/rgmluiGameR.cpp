#include <bridge/rgmluiGame.h>
#include <core/asset/rgmAssetFont.h>
#include <core/brush/rgmBrushStruct.h>
#include <core/util/rgmImpl.h>

/// <summary>
/// Initializes the resource.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::init_resource() noexcept {
    //assert(!"NOIMPL");
    // 添加默认资源 - 字体
    {
        alignas(void*) char buffer[sizeof(RubyGM::FontProperties) + sizeof(wchar_t) * MAX_PATH];
        auto& luidxtfprop = *reinterpret_cast<LongUI::DX::TextFormatProperties*>(buffer);
        auto& rgmfontprop = reinterpret_cast<RubyGM::FontProperties&>(luidxtfprop);
        LongUI::DX::InitTextFormatProperties(luidxtfprop, MAX_PATH);
        this->RegisterFontAsset(rgmfontprop);
    }
    // 添加默认资源 - 笔刷
    {
        RubyGM::NBrushProperties bnprop;
        RubyGM::MakeColorBrush(bnprop, impl::rubygm(D2D1::ColorF(D2D1::ColorF::Black)));
        this->RegisterBrushAsset(bnprop);
    }
}

/// <summary>
/// Adds the sprite.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::AddSprite(const SprteStatus& ss) noexcept -> CGMSprite* {
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
/// Sets the last error code.
/// </summary>
/// <param name="code">The code.</param>
/// <returns></returns>
void RubyGM::Game::SetLastErrorCode(uint32_t code) noexcept {
#ifdef _DEBUG
    HRESULT hr(code);
    assert(SUCCEEDED(hr) && "ERROR WITH HRESULT");
    hr = S_OK;
#endif
    RubyGM::Bridge::UIGame::GetInstance().error_code = code;
}


/// <summary>
/// Gets the detal time.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetDetalTime() noexcept -> float {
    return UIManager.GetDeltaTime();
}

/// <summary>
/// Gets the last error code.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetLastErrorCode() noexcept->uint32_t {
    return RubyGM::Bridge::UIGame::GetInstance().error_code;
}


/// <summary>
/// Gets the last resource object.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetLastResourceObject() noexcept -> Base::Resource* {
    return RubyGM::Bridge::UIGame::GetInstance().GetResourceTail();
}

/// <summary>
/// Gets the color brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetCommonBrush() noexcept ->IGMBrush* {
    auto brush = RubyGM::Bridge::UIGame::GetInstance().GetCommonBrush();
    return reinterpret_cast<IGMBrush*>(brush);
}


// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // get now time
    auto get_time() noexcept -> uint32_t { return ::timeGetTime(); }
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
auto RubyGM::Game::RefBitmapAsset(uint32_t index) 
noexcept -> Asset::Bitmap & {
    return Bridge::UIGame::GetInstance().RefBitmapAsset(index);
}


/// <summary>
/// Gets the font resource. it won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::RefFontAsset(uint32_t index) 
noexcept -> Asset::Font & {
    return Bridge::UIGame::GetInstance().RefFontAsset(index);
}

/// <summary>
/// Gets the brush resource. it won't add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::RefBrushAsset(uint32_t index) 
noexcept -> Asset::Brush & {
    return Bridge::UIGame::GetInstance().RefBrushAsset(index);
}

/// <summary>
/// Gets the bitmap resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetBitmapAsset(uint32_t index) 
noexcept -> Asset::Bitmap & {
    auto& asset = Bridge::UIGame::GetInstance().RefBitmapAsset(index);
    asset.AddRef();
    return asset;
}


/// <summary>
/// Gets the font resource. it will add ref-count
/// </summary>
/// <param name="index">The index.</param>
/// <returns></returns>
auto RubyGM::Game::GetFontAsset(uint32_t index) 
noexcept -> Asset::Font & {
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
auto RubyGM::Game::RegisterBrushAsset(const NBrushProperties& bnp) noexcept -> uint32_t {
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
auto RubyGM::Game::CreateFontWithProp(
    const FontProperties& fp, IGMFont** font) noexcept -> uint32_t {
    auto& prop = reinterpret_cast<const LongUI::DX::TextFormatProperties&>(fp);
    auto* tfmt = reinterpret_cast<IDWriteTextFormat**>(font);
    auto hr = LongUI::DX::CreateTextFormat(prop, tfmt);
    assert(SUCCEEDED(hr));
    return uint32_t(hr);
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
auto RubyGM::Game::CreateBrushWithProp(
    const NBrushProperties& prop, IGMBrush** brush) noexcept -> uint32_t {
    D2D1_BRUSH_PROPERTIES d2d1bprop;
    d2d1bprop.opacity = prop.opacity;
    d2d1bprop.transform = D2D1::Matrix3x2F::Identity();
    switch (prop.type)
    {
    case RubyGM::Type_Color:
        return uint32_t(UIManager_RenderTarget->CreateSolidColorBrush(
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
        return uint32_t(E_NOTIMPL);
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