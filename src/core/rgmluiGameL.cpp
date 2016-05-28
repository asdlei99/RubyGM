#include <bridge/rgmluiGame.h>
#include <core/util/rgmImpl.h>
#include <core/asset/rgmAsset.h>
#include <core/asset/rgmAssetFont.h>
#include <core/cache/rgmCache.h>
#include <core/graphics/rgmGraphics.h>
#include <LongUI/luiUiHlper.h>
#include <Graphics/luiGrWic.h>
#include <wincodec.h>

/// <summary>
/// Initializes a new instance of the <see cref="UIGame"/> class.
/// </summary>
/// <param name="cp">The cp.</param>
RubyGM::Bridge::UIGame::UIGame(LongUI::UIContainer* cp) noexcept :
Super(cp), 
m_sprRoot(RubyGM::DEFAULT_STATUS), 
m_strCacheDir(L"cache\\"),
m_uLastFrameTime(impl::get_time()) {
    std::memset(m_appAssetPtr, 0, sizeof(m_appAssetPtr));
    std::memset(m_acAssetPtr, 0, sizeof(m_acAssetPtr));
    assert(s_pInstance == nullptr && "only one instance");
    s_pInstance = this;
    std::memset(m_bufDrawaleHT, 0, sizeof(m_bufDrawaleHT));
    auto head = this->GetResourceHead();
    auto tail = this->GetResourceTail();
    head->m_pNext = tail;
    tail->m_pPrve = head;
    // 创建Wic工厂
    auto hr = ::CoCreateInstance(
        CLSID_WICImagingFactory2,
        nullptr,
        CLSCTX_INPROC_SERVER,
        LongUI_IID_PV_ARGS(m_pWicFactory)
    );
#if 0
    // 检查开头
    if (std::memcmp(m_strCacheDir.c_str(), LR"(\\?\)", 4 * sizeof(wchar_t))) {
        LongUI::CUIString str(LR"(\\?\)", 4);
        str += m_strCacheDir; m_strCacheDir = str;
        assert(m_strCacheDir.size() < size_t(INT16_MAX));
    }
#endif
    // 缓存测试
#ifdef _DEBUG
    Cache::WriteData(this, this, sizeof(*this));
#endif
    Game::SetLastErrorCode(Result(hr));
    // 申请重要内存失败则强行停止程序
    auto ptr = LongUI::NormalAllocT<void*>(MAX_SOURCE_EACH * RESOURCE_COUNT);
    if (!ptr) {
        UIManager.ShowError(E_OUTOFMEMORY);
        std::terminate();
    }
    // 清空数据
    std::memset(ptr, 0, sizeof(void*) * MAX_SOURCE_EACH * RESOURCE_COUNT);
    // 初始化数据
    for (size_t i = 0; i < RESOURCE_COUNT; ++i) {
        auto p = reinterpret_cast<Asset::Object**>(ptr);
        m_appAssetPtr[i] = p + MAX_SOURCE_EACH * i;
    }
    // 初始化资源
    this->init_resource();
}


/// <summary>
/// Finalizes an instance of the <see cref="UIGame"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Bridge::UIGame::~UIGame() noexcept {
    // 释放WIC工厂
    RubyGM::SafeRelease(m_pWicFactory);
    // 释放精灵
    m_sprRoot.Clear();
    // 释放资源数据
    this->release_asset();
    // 释放设备相关资源
    this->release_gpu_data();
    // 释放资源指针数据
    assert(*m_appAssetPtr && "bad action");
    LongUI::NormalFree(*m_appAssetPtr);
    std::memset(m_appAssetPtr, 0, sizeof(m_appAssetPtr));
    std::memset(m_acAssetPtr, 0, sizeof(m_acAssetPtr));
#ifdef _DEBUG
    auto head = this->GetResourceHead();
    auto tail = this->GetResourceTail();
    assert(head->m_pNext == tail && "bad object releasing");
    assert(tail->m_pPrve == head && "bad node linking");
#endif
    // 检查资源释放情况
    if (m_pSubFiber) {
        ::DeleteFiber(m_pSubFiber);
        m_pSubFiber = nullptr;
    }
    m_pMainFiber = nullptr;
    assert(s_pInstance == this && "only one instance");
    s_pInstance = nullptr;
}


/// <summary>
/// Renders this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::Render() const noexcept {
    // 渲染呈现器目标
    auto rc = UIManager_RenderTarget;
    // 清理颜色
    rc->Clear(D2D1::ColorF(D2D1::ColorF::LightCyan, 0.95f));
    // 渲染根精灵
    m_sprRoot.RootRender(*reinterpret_cast<IGMRenderContext*>(rc));
}

// rubygm::updatefiber
namespace RubyGM { void UpdateFiber() noexcept; }

/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::Update() noexcept {
    // 创建
    if (!m_pMainFiber) {
        auto update_fiber = [](void*) { RubyGM::UpdateFiber(); };
        m_pMainFiber = ::ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH);
        m_pSubFiber = ::CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, update_fiber, m_pMainFiber);
    }
    // 刷新转换矩阵
    if (this->world._11 != m_sprRoot.GetTransform()._11) {
        m_sprRoot.SetZoomX(this->world._11);
        m_sprRoot.SetZoomY(this->world._22);
        m_sprRoot.SetTransform(impl::rubygm(this->world));
        UIManager << DL_Log
            << L"Root Sprite : SetTransform"
            << LongUI::endl;
    }
    // 切换到子纤程
    if (m_pSubFiber) { ::SwitchToFiber(m_pSubFiber); }
    // 其他操作
    this->InvalidateThis();
    //m_pWindow->InvalidateWindow();
    // 适配
    return Super::Update();
}

/// <summary>
/// Releases the gpu data.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::release_gpu_data() noexcept {
    LongUI::SafeRelease(m_pCommonBrush);
}


/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::Recreate() noexcept -> HRESULT {
    this->release_gpu_data();
    HRESULT hr = S_OK;
    Game::SetLastErrorCode(Result(S_OK));
    // 创建公共笔刷
    if (SUCCEEDED(hr)) {
        auto brush = this->RefBrushAsset(0).GetBrush();
        m_pCommonBrush = reinterpret_cast<decltype(m_pCommonBrush)>(brush);
#ifdef _DEBUG
        {
            IUnknown* test = nullptr;
            m_pCommonBrush->QueryInterface(IID_ID2D1SolidColorBrush, (void**)(&test));
            assert(test && "bad type for brush");
            RubyGM::SafeRelease(test);
        }
#endif
    }
    // 调试
#ifdef _DEBUG
    assert(SUCCEEDED(hr) && "pre-assert failed #1");
#endif
    // 重建资源: 去掉头尾
    {
        auto res = this->GetResourceTail()->m_pPrve;
        while (res->m_pPrve) {
            hr = static_cast<HRESULT>(res->Recreate());
            if (FAILED(hr)) break;
            res = res->m_pPrve;
        }
    }
    // 结束重建
    {
        auto res = this->GetResourceTail()->m_pPrve;
        while (res->m_pPrve) {
            res->AfterRecreate();
            res = res->m_pPrve;
        }
    }
    // 调试
#ifdef _DEBUG
    assert(SUCCEEDED(hr) && "pre-assert failed #2");
#endif
    // 检查错误代码
    if (SUCCEEDED(hr)) {
        hr = static_cast<HRESULT>(Game::GetLastErrorCode());
    }
    // 调试
#ifdef _DEBUG
    assert(SUCCEEDED(hr) && "pre-assert failed #3");
#endif
    // 父类重建
    if (SUCCEEDED(hr)) {
        hr = Super::Recreate();
    }
    return hr;
}

/// <summary>
/// Before_deleteds this instance.
/// </summary>
void RubyGM::Bridge::UIGame::before_deleted() noexcept { 
    // 清除缓存
    Cache::RemoveAllFiles();
    // 链式调用
    Super::before_deleted(); 
}

/// <summary>
/// Cleanups this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Bridge::UIGame::cleanup() noexcept {
    // 删前调用
    this->before_deleted();
    // 删除对象
    delete this;
}

/// <summary>
/// Initializes the specified node.
/// </summary>
/// <param name="node">The node.</param>
/// <returns></returns>
void RubyGM::Bridge::UIGame::initialize(pugi::xml_node node) noexcept {
    // 链式初始化
    Super::initialize(node);
}


/// <summary>
/// Creates the game control.
/// </summary>
/// <param name="type">The type.</param>
/// <param name="node">The node.</param>
/// <returns></returns>
auto RubyGM::Bridge::UIGame::CreateControl(
    LongUI::CreateEventType type, 
    pugi::xml_node node) noexcept -> LongUI::UIControl * {
    UIGame* pControl = nullptr;
    switch (type)
    {
    case LongUI::Type_Initialize:
        break;
    case LongUI::Type_Recreate:
        break;
    case LongUI::Type_Uninitialize:
        break;
    case_LongUI__Type_CreateControl:
        LongUI__CreateWidthCET(UIGame, pControl, type, node);
    }
    return pControl;
}


#ifdef LongUIDebugEvent
/// <summary>
/// debug infomation for <see cref="UIGame"/> 
/// </summary>
/// <param name="info">The information.</param>
/// <returns></returns>
bool RubyGM::Bridge::UIGame::debug_do_event(
    const LongUI::DebugEventInformation& info) const noexcept {
    switch (info.infomation)
    {
    case LongUI::DebugInformation::Information_GetClassName:
        info.str = L"UIGame";
        return true;
    case LongUI::DebugInformation::Information_GetFullClassName:
        info.str = L"::RubyGM::Bridge::UIGame";
        return true;
    case LongUI::DebugInformation::Information_CanbeCasted:
        // 类型转换
        return *info.iid == LongUI::GetIID<UIGame>()
            || Super::debug_do_event(info);
    default:
        break;
    }
    return false;
}
#endif


/// <summary>
/// Sets the last error code.
/// </summary>
/// <param name="code">The code.</param>
/// <returns></returns>
void RubyGM::Game::SetLastErrorCode(Result code) noexcept {
#ifdef _DEBUG
    HRESULT hr(code);
    if (FAILED(hr)) UIManager.ShowError(hr);
    hr = S_OK;
#endif
    RubyGM::Bridge::UIGame::GetInstance().error_code = code;
}

/// <summary>
/// Gets the detal time.
/// </summary>
/// <returns></returns>
auto RubyGM::Game::GetDetalTime() noexcept -> float {
    auto time = UIManager.GetDeltaTime();
    auto delta = time > 0.5f ? 0.f : time;
    auto real = delta * Game::GetTimeScale();
    return real;
}

// rubygm::cache namespace
namespace RubyGM { namespace Cache {
    /// <summary>
    /// Gets the directory name for cache
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="len">The length.</param>
    /// <returns></returns>
    auto GetDirName(wchar_t path[], size_t len) noexcept -> wchar_t* {
        //LongUI::CUIDataAutoLocker locker;
        const auto& str = Bridge::UIGame::GetInstance().GetCacheDir();
        if (str.size() >= len) return nullptr;
        const size_t leninbyte = (str.size() + 1) * sizeof(wchar_t);
        std::memcpy(path, str.c_str(), leninbyte);
        return path + str.size();
    }
    /// <summary>
    /// Gets the length of the dir name.
    /// </summary>
    /// <returns></returns>
    auto GetDirNameLength() noexcept -> uint32_t {
        //LongUI::CUIDataAutoLocker locker;
        const auto& str = Bridge::UIGame::GetInstance().GetCacheDir();
        return str.length() + 1;
    }
}}


#ifdef _DEBUG
#include <map>
#include <cassert>
namespace RubyGM { namespace impl {
    // debugger
    struct alloc_debugger {
        alloc_debugger() {}
        ~alloc_debugger() noexcept { assert(map.empty()); }
        void alloc(void* ptr, size_t len) { 
            assert(map.find(ptr) == map.end());
            try { map.insert(std::pair<void*, size_t>(ptr, len)); }
            catch (...) { assert(!"error"); }
        }
        void free(void* ptr) { 
            assert(map.find(ptr) != map.end());
            try { map.erase(ptr); }
            catch (...) { assert(!"error"); }
        }
        std::map<void*, size_t> map;
    };
    // buffer
    alignas(void*) char g_dbg_small_alloc_data[sizeof(alloc_debugger)];
    // get_alloc_debugger
    static auto&get_alloc_debugger() noexcept { 
        return *reinterpret_cast<alloc_debugger*>(g_dbg_small_alloc_data);
    }
    // init
    void init() {
        get_alloc_debugger().alloc_debugger::alloc_debugger();
    }
    // uninit
    void uninit() {
        get_alloc_debugger().alloc_debugger::~alloc_debugger();
    }
}}
#endif

/// <summary>
/// Smalls the alloc a small size(less 256)
/// </summary>
/// <param name="size">The size.</param>
/// <returns></returns>
auto RubyGM::SmallAlloc(size_t len) noexcept -> void * {
    assert(len <= 256);
    auto ptr = LongUI::SmallAlloc(len);
#ifdef _DEBUG
    impl::get_alloc_debugger().alloc(ptr, len);
#endif
    return ptr;
}

/// <summary>
/// free memory return from SmallAlloc;
/// </summary>
/// <param name="">The ptr</param>
/// <returns></returns>
void RubyGM::SmallFree(void* ptr) noexcept {
#ifdef _DEBUG
    impl::get_alloc_debugger().free(ptr);
#endif
    return LongUI::SmallFree(ptr);
}


/// <summary>
/// Allocs the specified .
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::NormalAlloc(size_t len) noexcept -> void* {
    return LongUI::NormalAlloc(len);
}

/// <summary>
/// Frees the specified .
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void RubyGM::NormalFree(void* ptr) noexcept {
    LongUI::NormalFree(ptr);
}

// rubygm::resouce namepsace
namespace RubyGM { namespace Asset {
    // create text format
    auto CreateTextFormat(
        const wchar_t* name, 
        float size, 
        IDWriteTextFormat** ptr) noexcept -> Result {
        // XXX: 去除硬编码
        assert(!"");
        return UIManager.CreateTextFormat(
            name,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            size,
            ptr
        );
    }
    // create text layout
    auto CreateTextLayout(
        const wchar_t* str, 
        uint32_t len,
        IDWriteTextFormat* fmt, 
        IDWriteTextLayout** ptr) noexcept -> Result {
        // XXX: 去除硬编码
        auto hr = UIManager_DWriteFactory->CreateTextLayout(
            str, len, fmt, 320.f, 320.f, ptr
        );
        return Result(hr);
    }
}}


// longui::impl 命名空间
namespace LongUI { namespace impl {
    // load bitmap
    auto load_bitmap_from_file(
        ID2D1DeviceContext* pRenderTarget,
        IWICImagingFactory* pIWICFactory,
        PCWSTR uri,
        UINT destinationWidth,
        UINT destinationHeight,
        ID2D1Bitmap1 **ppBitmap
    ) noexcept->HRESULT;
}}

// rubygm::impl 命名空间
namespace RubyGM { namespace impl {
    /// <summary>
    /// Creates the typography.
    /// </summary>
    /// <param name="typography">The typography</param>
    /// <returns></returns>
    auto create_typography(IDWriteTypography** typography) noexcept->Result {
        return Result(UIManager_DWriteFactory->CreateTypography(typography));
    }
    // create bitmap render target
    auto create_bitmap_render_target(SizeF size, 
        ID2D1BitmapRenderTarget** brt) noexcept -> Result {
        D2D1_PIXEL_FORMAT format = D2D1::PixelFormat(
            DXGI_FORMAT_B8G8R8A8_UNORM, 
            D2D1_ALPHA_MODE_PREMULTIPLIED
        );
        D2D1_SIZE_F fs { size.width, size.height };
        auto hr = UIManager_RenderTarget->CreateCompatibleRenderTarget(
            &fs,
            nullptr,
            &format,
            D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
            brt
        );
        return Result(hr);
    }
    // get iid of render context
    auto get_rendercontext_iid() noexcept -> const IID&{
        return LongUI::GetIID<IGMRenderContext::Super>();
    };
    // get iid of bitmap
    auto get_bitmap_iid()noexcept -> const IID&{
        return LongUI::GetIID<IGMBitmap::Super>();
    };
}}


// rubygm::dx namespace
namespace RubyGM { namespace DX {
    // create bitmap from file
    auto CreateBitmapFromFile(const wchar_t* url, 
        IGMBitmap*& bitmap) noexcept -> Result {
        auto* wic = &Bridge::UIGame::GetInstance().RefWicFactory();
        if (!wic) return Result(E_FAIL);
        return Result(LongUI::impl::load_bitmap_from_file(
            UIManager_RenderTarget, wic, url,0,0,
            reinterpret_cast<ID2D1Bitmap1**>(&bitmap)
        ));
    }
}}


/// <summary>
/// Saves as PNG.
/// </summary>
/// <param name="file_name">The file_name.</param>
/// <returns></returns>
auto RubyGM::Asset::Bitmap::SaveAsPng(
    const wchar_t* file_name) noexcept ->Result {
    // 先决错误处理
    if (!(file_name && *file_name)) return Result(E_INVALIDARG);
    auto bitmap = this->GetBitmap();
    if (!bitmap) return Result(E_FAIL);
    // 保存数据
    auto hr = LongUI::DX::SaveAsImageFile(
        bitmap, 
        &Bridge::UIGame::GetInstance().GetInstance().RefWicFactory(), 
        file_name,
        &GUID_ContainerFormatPng
    );
    // 释放位图
    bitmap->Release();
    return Result(hr);
}

/// <summary>
/// Saves as PNG.
/// </summary>
/// <param name="file_name">The file_name.</param>
/// <returns></returns>
auto RubyGM::Asset::Bitmap::SaveAsPng(
    const char* file_name) noexcept -> Result {
    // 没有调用只有一种情况就是内存不足
    Result hr = E_OUTOFMEMORY;
    auto ptr = this;
    // 安全转换字符串
    LongUI::SafeUTF8toWideChar(file_name, 
        [ptr, &hr](const wchar_t* bgn, const wchar_t* end) noexcept {
        hr = ptr->SaveAsPng(bgn);
    });
    // 返回结果
    return hr;
}


// instance for game
RubyGM::Bridge::UIGame* RubyGM::Bridge::UIGame::s_pInstance = nullptr;


// rubygm namespace
namespace RubyGM {
    // yield for fiber
    void FiberYieldBasic() noexcept {
        if (const auto ctx = ::GetFiberData()) 
            ::SwitchToFiber(ctx);
    }
    // da
    struct exit_exp { const char* info; SSIZE_T code; };
    // yield for fiber
    void FiberYield() throw(exit_exp) {
        if (UIManager.IsExit()) { 
            throw(exit_exp{"exit", 0});
        }
        if (const auto ctx = ::GetFiberData()) {
            ::SwitchToFiber(ctx);
        }
        else {
            throw(exit_exp{"bad ctx", 1});
        }
    }
}


