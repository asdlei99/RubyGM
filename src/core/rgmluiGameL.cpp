#include <bridge/rgmluiGame.h>
#include <core/util/rgmImpl.h>
#include <core/asset/rgmAsset.h>
#include <core/asset/rgmAssetFont.h>
#include <algorithm>

/// <summary>
/// Initializes a new instance of the <see cref="UIGame"/> class.
/// </summary>
/// <param name="cp">The cp.</param>
RubyGM::Bridge::UIGame::UIGame(LongUI::UIContainer* cp) 
noexcept : Super(cp), m_sprRoot(RubyGM::DEFAULT_STATUS, nullptr), 
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
}

/// <summary>
/// Finalizes an instance of the <see cref="UIGame"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Bridge::UIGame::~UIGame() noexcept {
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
    assert(head->m_pNext == tail && "bad object relesing");
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
    rc->Clear(D2D1::ColorF(D2D1::ColorF::White));
    // 渲染根精灵
    m_sprRoot.RootRender(*reinterpret_cast<IGMRednerContext*>(rc));
}

// rubygm::updatefiber
namespace RubyGM { void UpdateFiber(); }

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
    // 切换到子纤程
    if (m_pSubFiber) { ::SwitchToFiber(m_pSubFiber); }
    // 其他操作
    this->InvalidateThis();
    m_sprRoot.SetTransform(impl::rubygm(this->world));
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
    Game::SetLastErrorCode(uint32_t(S_OK));
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
/// Smalls the alloc.
/// </summary>
/// <param name="size">The size.</param>
/// <returns></returns>
auto RubyGM::SmallAlloc(size_t len) noexcept -> void * {
    assert(len <= 256);
    return LongUI::SmallAlloc(len);
}

/// <summary>
/// Smalls the free.
/// </summary>
/// <param name="">The ptr</param>
/// <returns></returns>
void RubyGM::SmallFree(void* ptr) noexcept {
    return LongUI::SmallFree(ptr);
}

// rubygm::resouce namepsace
namespace RubyGM { namespace Asset {
    // create text format
    auto CreateTextFormat(const wchar_t* name, float size, IDWriteTextFormat** ptr) noexcept {
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
    auto CreateTextLayout(const wchar_t* n, uint32_t l,IDWriteTextFormat* f, IDWriteTextLayout** ptr) noexcept {
        return UIManager_DWriteFactory->CreateTextLayout(n, l, f, 320.f, 320.f, ptr);
    }
}}



// instance for game
RubyGM::Bridge::UIGame* RubyGM::Bridge::UIGame::s_pInstance = nullptr;


// rubygm namespace
namespace RubyGM {
    // yield for fiber
    void FiberYield() {
        auto data = ::GetFiberData();
        if (data) {
            ::SwitchToFiber(data);
        }
        else {
            assert(!"NOIMPL");
        }
    }
}


