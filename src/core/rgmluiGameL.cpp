#include <bridge/rgmluiGame.h>
#include <core/util/rgmImpl.h>

/// <summary>
/// Initializes a new instance of the <see cref="UIGame"/> class.
/// </summary>
/// <param name="cp">The cp.</param>
RubyGM::Bridge::UIGame::UIGame(LongUI::UIContainer* cp) 
noexcept : Super(cp), m_sprRoot(nullptr), 
m_uLastFrameTime(impl::get_time()) {
    assert(s_pInstance == nullptr && "only one instance");
    s_pInstance = this;
    std::memset(m_bufDrawaleHT, 0, sizeof(m_bufDrawaleHT));
    auto head = this->GetDrawableHead();
    auto tail = this->GetDrawableTail();
    head->LinkNext(tail);
    tail->LinkPrve(head);
}

/// <summary>
/// Finalizes an instance of the <see cref="UIGame"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Bridge::UIGame::~UIGame() noexcept {
    this->release_gpu_data();
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
    // 创建公共笔刷
    if (SUCCEEDED(hr)) {
        hr = UIManager_RenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_pCommonBrush
        );
    }
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
namespace RubyGM { namespace Resource {
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


