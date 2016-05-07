#define _WIN32_WINNT 0x0A01
#include <Windows.h>
#include <dwrite_1.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <d3d11.h>
#include <core/rubygm.h>
#include <cstdint>
#include <cassert>
#include <cwchar>
#include <ShellScalingAPI.h>
#include <core/graphics/rgmSprite.h>
#include <core/graphics/rgmGraphics.h>

// rubygm namespace
namespace RubyGM {
    // UpdateFiber
    void UpdateFiber();
    // impl namespace
    namespace impl {
        // get now time
        auto get_time() noexcept -> uint32_t { return ::timeGetTime(); }
    }
    // class name
    const wchar_t* const WindowClassName = L"Windows.UI.RubyGM.SystemInvoke";
    // title name
    const wchar_t* const WindowClassTitle = L"RubyGM - Test";
    // IDWriteFactory1 ("30572f99-dac6-41db-a16e-0486307e606a")
    const GUID IID_IDWriteFactory1 = {
        0x30572f99, 0xdac6, 0x41db, { 0xa1, 0x6e, 0x04, 0x86, 0x30, 0x7e, 0x60, 0x6a }
    };
    // framework
    class CGMFramework final {
    public:
        // set
        static auto& GetInstance() noexcept { assert(s_pInstance); return *s_pInstance; }
    public:
        // add sprite to top level
        auto AddSprite() noexcept->CGMSprite*;
        // get common brush
        auto GetCommonBrush() const noexcept { return RubyGM::SafeAcquire(m_pCommonBrush); }
    public:
        // ctor
        CGMFramework() noexcept;
        // dtor
        ~CGMFramework() noexcept { this->release_data(); }
        // init
        auto Initialize(uint32_t width, uint32_t height) noexcept -> HRESULT;
        // recreate resource
        auto Recreate() noexcept -> HRESULT;
        // render for one frame
        auto Render() noexcept -> HRESULT;
        // run
        void Run() noexcept;
    public:
        // get drawable head
        auto GetDrawableHead() const { return this->get_drawable<Drawable::Base, 0>(); }
        // get drawable tail
        auto GetDrawableTail() const { return this->get_drawable<Drawable::Base, 1>(); }
    public:
        // message handle
        bool MessageHandle(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result) noexcept;
        // window proc
        static auto WINAPI WndProc(HWND , UINT , WPARAM , LPARAM ) noexcept->LRESULT;
    private:
        // release data
        void release_data() noexcept;
        // discard dxgi gpu resource
        void discard_gpu_resource() noexcept;
        // discard dxgi cpu resource
        void discard_cpu_resource() noexcept;
        // recreate fps layout
        void recreate_fps_layout() noexcept;
        // get drawable
        template<typename T, size_t U> 
        auto get_drawable() const noexcept { return (T*)(m_bufDrawaleHT + sizeof(T) * U); }
    private:
        // handle for window
        HWND                        m_hwnd = nullptr;
        // wait for vblank event
        HANDLE                      m_hVBlank = nullptr;
    private:
        // main format
        IDWriteTextFormat*          m_pTextFormatMain = nullptr;
        // FPS display
        IDWriteTextLayout*          m_pFPSLayout = nullptr;
        // D2D Factory
        ID2D1Factory4*              m_pd2dFactory = nullptr;
        // DWrite Factory
        IDWriteFactory1*            m_pDWriteFactory = nullptr;
    private:
        // D3D Device
        ID3D11Device*               m_pd3dDevice = nullptr;
        // D3D Device Context
        ID3D11DeviceContext*        m_pd3dDeviceContext = nullptr;
        // D2D Device
        ID2D1Device3*               m_pd2dDevice = nullptr;
        // D2D Device Context
        ID2D1DeviceContext3*        m_pd2dDeviceContext = nullptr;
        // DXGI Factory
        IDXGIFactory2*              m_pDxgiFactory = nullptr;
        // DXGI Device
        IDXGIDevice1*               m_pDxgiDevice = nullptr;
        // DXGI Adapter
        IDXGIAdapter*               m_pDxgiAdapter = nullptr;
        // swap chain
        IDXGISwapChain2*            m_pSwapChain    = nullptr;
        // target bitmap
        ID2D1Bitmap1*               m_pTargetBitmap = nullptr;
        // basic brush
        ID2D1SolidColorBrush*       m_pBaiscBrush = nullptr;
        // common brush
        ID2D1SolidColorBrush*       m_pCommonBrush = nullptr;
    private:
        // feature level
        D3D_FEATURE_LEVEL           m_featureLevel = D3D_FEATURE_LEVEL_11_0;
        // frame count
        uint32_t                    m_uFrameCount = 0;
        // window width
        uint32_t                    m_uWndWidth = 0;
        // window height
        uint32_t                    m_uWndHeight = 0;
        // last frame time count
        uint32_t                    m_uLastFrameTime = impl::get_time();
        // now fps
        float                       m_fps = 0.f;
        // unused
        uint32_t                    unused = 0;
    public:
        // error code
        uint32_t                    error_code = 0;
    private:
        // sprite list
        RubyGM::List<CGMSprite>     m_ltSprites;
        // buffer for drawable head and tail
        char                        m_bufDrawaleHT[sizeof(Drawable::Base) * 2];
    private:
        // single
        static CGMFramework*        s_pInstance;
    };
}

/// <summary>
/// Adds the new.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::CGMSprite::AddNew(const SprteStatus& status) noexcept -> CGMSprite* {
    UNREFERENCED_PARAMETER(status);
    // TODO: status to sprite
    return RubyGM::CGMFramework::GetInstance().AddSprite();
}

/// <summary>
/// Sets the last error code.
/// </summary>
/// <param name="code">The code.</param>
/// <returns></returns>
void RubyGM::SetLastErrorCode(uint32_t code) noexcept {
    RubyGM::CGMFramework::GetInstance().error_code = code;
}

/// <summary>
/// Gets the last error code.
/// </summary>
/// <returns></returns>
auto RubyGM::GetLastErrorCode() noexcept->uint32_t {
    return RubyGM::CGMFramework::GetInstance().error_code;
}


/// <summary>
/// Uninitializes this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::GetLastDrawableObject() noexcept -> Drawable::Base* {
    return RubyGM::CGMFramework::GetInstance().GetDrawableTail();
}

/// <summary>
/// Gets the color brush.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::GetColorBrush() noexcept ->IGMBrush* {
    auto brush = RubyGM::CGMFramework::GetInstance().GetCommonBrush();
    return reinterpret_cast<IGMBrush*>(brush);
}

// init
RubyGM::CGMFramework*  RubyGM::CGMFramework::s_pInstance = nullptr;

int main() {
    ::SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    {
        RubyGM::CGMFramework framework;
        framework.Initialize(1024, 768);
        framework.Run();
    }
    return 0;
}


/// <summary>
/// Initializes a new instance of the <see cref="CGMFramework"/> class.
/// </summary>
RubyGM::CGMFramework::CGMFramework() noexcept {
    assert(s_pInstance == nullptr);
    s_pInstance = this;
    std::memset(m_bufDrawaleHT, 0, sizeof(m_bufDrawaleHT));
    auto head = this->GetDrawableHead();
    auto tail = this->GetDrawableTail();
    head->m_pNext = tail;
    tail->m_pPrve = head;
}

/// <summary>
/// Release_datas this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMFramework::release_data() noexcept {
    // 释放精灵列表
    try { m_ltSprites.clear(); } catch (...) {};
    // 释放 Dxgi 的GPU资源
    this->discard_gpu_resource();
    // 释放 Dxgi 的CPU资源
    this->discard_cpu_resource();
    // 释放
    if (m_hVBlank) {
        ::CloseHandle(m_hVBlank);
        m_hVBlank = nullptr;
    }
}

/// <summary>
/// Discard dxgi gpu resources for this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMFramework::discard_gpu_resource() noexcept {
    RubyGM::SafeRelease(m_pd2dDeviceContext);
    RubyGM::SafeRelease(m_pd3dDeviceContext);
    RubyGM::SafeRelease(m_pTargetBitmap);
    RubyGM::SafeRelease(m_pDxgiFactory);
    RubyGM::SafeRelease(m_pDxgiAdapter);
    RubyGM::SafeRelease(m_pCommonBrush);
    RubyGM::SafeRelease(m_pBaiscBrush);
    RubyGM::SafeRelease(m_pDxgiDevice);
    RubyGM::SafeRelease(m_pd2dDevice);
    RubyGM::SafeRelease(m_pSwapChain);
    RubyGM::SafeRelease(m_pd3dDevice);
}

/// <summary>
/// Discard dxgi cpu resources for this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMFramework::discard_cpu_resource() noexcept {
    RubyGM::SafeRelease(m_pTextFormatMain);
    RubyGM::SafeRelease(m_pDWriteFactory);
    RubyGM::SafeRelease(m_pd2dFactory);
    RubyGM::SafeRelease(m_pFPSLayout);
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMFramework::Recreate() noexcept -> HRESULT {
    // 创建设备资源
    this->discard_gpu_resource();
    HRESULT hr = S_OK;
    // 创建 D3D11设备与设备上下文 
    if (SUCCEEDED(hr)) {
        // D3D11 创建flag 
        // 一定要有D3D11_CREATE_DEVICE_BGRA_SUPPORT
        // 否则创建D2D设备上下文会失败
        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
        // Debug状态 有D3D DebugLayer就可以取消注释
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
        auto tmpflag = D3D11_CREATE_DEVICE_FLAG(creationFlags);
        tmpflag = D3D11_CREATE_DEVICE_FLAG(0);
#endif
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };
        // 创建设备
        hr = ::D3D11CreateDevice(
            // 设置为渲染
            nullptr,
            // 驱动类型
            D3D_DRIVER_TYPE_HARDWARE,
            // 没有软件接口
            nullptr,
            // 创建flag
            creationFlags,
            // 欲使用的特性等级列表
            featureLevels,
            // 特性等级列表长度
            static_cast<UINT>(sizeof(featureLevels)/sizeof(featureLevels)),
            // SDK 版本
            D3D11_SDK_VERSION,
            // 返回的D3D11设备指针
            &m_pd3dDevice,
            // 返回的特性等级
            &m_featureLevel,
            // 返回的D3D11设备上下文指针
            &m_pd3dDeviceContext
        );
        // 检查
        if (FAILED(hr)) {
            // L"create d3d11 device failed, now, try to create in warp mode";
        }
        // 创建失败则尝试使用软件
        if (FAILED(hr)) {
            hr = ::D3D11CreateDevice(
                // 设置为渲染
                nullptr,
                // 根据情况选择类型
                D3D_DRIVER_TYPE_WARP,
                // 没有软件接口
                nullptr,
                // 创建flag
                creationFlags,
                // 欲使用的特性等级列表
                featureLevels,
                // 特性等级列表长度
                static_cast<UINT>(sizeof(featureLevels)/sizeof(featureLevels)),
                // SDK 版本
                D3D11_SDK_VERSION,
                // 返回的D3D11设备指针
                &m_pd3dDevice,
                // 返回的特性等级
                &m_featureLevel,
                // 返回的D3D11设备上下文指针
                &m_pd3dDeviceContext
            );
        }
        // 再次检查错误
        if (FAILED(hr)) {
            // L" create d3d11-device in warp modd, but failed."
        }
    }
    // 创建 IDXGIDevice
    if (SUCCEEDED(hr)) {
        hr = m_pd3dDevice->QueryInterface(
            IID_IDXGIDevice1,
            reinterpret_cast<void**>(&m_pDxgiDevice)
        );
    }
    // 创建 D2D设备
    if (SUCCEEDED(hr)) {
        hr = m_pd2dFactory->CreateDevice(m_pDxgiDevice, &m_pd2dDevice);
    }
    // 创建 D2D设备上下文
    if (SUCCEEDED(hr)) {
        hr = m_pd2dDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            &m_pd2dDeviceContext
        );
    }
    // 获取 Dxgi适配器 可以获取该适配器信息
    if (SUCCEEDED(hr)) {
        // 顺带使用像素作为单位
        m_pd2dDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
        // 获取
        hr = m_pDxgiDevice->GetAdapter(&m_pDxgiAdapter);
    }
    // 获取 Dxgi工厂
    if (SUCCEEDED(hr)) {
        hr = m_pDxgiAdapter->GetParent(
            IID_IDXGIFactory2,
            reinterpret_cast<void**>(&m_pDxgiFactory)
        );
    }
    // DXGI Surface 后台缓冲
    IDXGISurface*               pDxgiBackBuffer = nullptr;
    IDXGISwapChain1*            pSwapChain = nullptr;
    // 创建交换链
    if (SUCCEEDED(hr)) {
        // 交换链信息
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
        swapChainDesc.Width = m_uWndWidth;
        swapChainDesc.Height = m_uWndHeight;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        // 一般桌面应用程序
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        // 利用窗口句柄创建交换链
        hr = m_pDxgiFactory->CreateSwapChainForHwnd(
            m_pd3dDevice,
            m_hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &pSwapChain
        );
    }
    // 获取交换链V2
    if (SUCCEEDED(hr)) {
        hr = pSwapChain->QueryInterface(
            IID_IDXGISwapChain2,
            reinterpret_cast<void**>(&m_pSwapChain)
        );
    }
    // 获取垂直等待事件
    if (SUCCEEDED(hr)) {
        m_hVBlank = m_pSwapChain->GetFrameLatencyWaitableObject();
    }
    // 确保DXGI队列里边不会超过一帧
    if (SUCCEEDED(hr)) {
        //hr = UIManager_DXGIDevice->SetMaximumFrameLatency(1);
        //longui_debug_hr(hr, L"UIManager_DXGIDevice->SetMaximumFrameLatency faild");
    }
    // 利用交换链获取Dxgi表面
    if (SUCCEEDED(hr)) {
        hr = m_pSwapChain->GetBuffer(
            0, 
            IID_IDXGISurface,
            reinterpret_cast<void**>(&pDxgiBackBuffer)
        );
    }
    // 利用Dxgi表面创建位图
    if (SUCCEEDED(hr)) {
        D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );
        hr = m_pd2dDeviceContext->CreateBitmapFromDxgiSurface(
            pDxgiBackBuffer,
            &bitmapProperties,
            &m_pTargetBitmap
        );
    }
    // 扫尾处理
    RubyGM::SafeRelease(pDxgiBackBuffer);
    RubyGM::SafeRelease(pSwapChain);
    // 创建笔刷
    if (SUCCEEDED(hr)) {
        hr = m_pd2dDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_pBaiscBrush
        );
    }
    // 创建笔刷
    if (SUCCEEDED(hr)) {
        hr = m_pd2dDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_pCommonBrush
        );
    }
    // 设置
    if (SUCCEEDED(hr)) {
        m_pd2dDeviceContext->SetTarget(m_pTargetBitmap);
    }
    return hr;
}


/// <summary>
/// Initializes the specified width.
/// </summary>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <returns></returns>
auto RubyGM::CGMFramework::Initialize(uint32_t width, uint32_t height) noexcept -> HRESULT {
    // 结果
    this->discard_cpu_resource();
    auto hr = S_OK;
    // 创建D2D工厂
    if (SUCCEEDED(hr)) {
        D2D1_FACTORY_OPTIONS options = { D2D1_DEBUG_LEVEL_NONE };
#ifdef _DEBUG
        options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
        hr = ::D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            IID_ID2D1Factory4,
            &options,
            reinterpret_cast<void**>(&m_pd2dFactory)
        );
    }
    // 创建 DirectWrite 工厂.
    if (SUCCEEDED(hr)) {
        hr = ::DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            RubyGM::IID_IDWriteFactory1,
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
        );
    }
    // 创建正文文本格式.
    if (SUCCEEDED(hr)) {
        hr = m_pDWriteFactory->CreateTextFormat(
            L"Courier New",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            22.f,
            L"en-us",
            &m_pTextFormatMain
        );
    }
    // 注册窗口类
    if (SUCCEEDED(hr)) {
        // 注册一般窗口类
        WNDCLASSEXW wcex = { 0 };
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = 0;
        wcex.lpfnWndProc = CGMFramework::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(void*);
        wcex.hInstance = ::GetModuleHandleW(nullptr);
        wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = RubyGM::WindowClassName;
        wcex.hIcon = nullptr;
        auto code = ::RegisterClassExW(&wcex);
        // 检查返回值
        if (!code) {
            hr = HRESULT_FROM_WIN32(::GetLastError());
        }
    }
    // 创建窗口
    if (SUCCEEDED(hr)) {
        // 检查样式样式
        constexpr DWORD window_style = WS_OVERLAPPEDWINDOW;
        // 设置窗口大小
        D2D1_RECT_L window_rect;
        window_rect.left = 0;
        window_rect.top = 0;
        window_rect.right = m_uWndWidth = width;
        window_rect.bottom = m_uWndHeight = height;
        // 调整大小
        ::AdjustWindowRect(&window_rect, window_style, FALSE);
        // 窗口
        window_rect.right = (window_rect.right - window_rect.left);
        window_rect.bottom = (window_rect.bottom - window_rect.top);
        window_rect.left = (::GetSystemMetrics(SM_CXFULLSCREEN) - window_rect.right) / 2;
        window_rect.top = (::GetSystemMetrics(SM_CYFULLSCREEN) - window_rect.bottom) / 2;
        // 创建窗口
        m_hwnd = ::CreateWindowExW(
            //WS_EX_NOREDIRECTIONBITMAP | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
            0,
            RubyGM::WindowClassName,
            RubyGM::WindowClassTitle,
            window_style,
            window_rect.left, window_rect.top, window_rect.right, window_rect.bottom,
            nullptr,
            nullptr,
            ::GetModuleHandleW(nullptr),
            this
        );
        // 创建成功: 重建资源
        if (m_hwnd) {
            hr = this->Recreate();
        }
        // 创建失败: 检查错误代码
        else {
            hr = HRESULT_FROM_WIN32(::GetLastError());
        }
    }
    // 禁止 Alt + Enter 全屏
    if (SUCCEEDED(hr)) {
        hr = m_pDxgiFactory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
    }
    return hr;
}



/// <summary>
/// WNDs the proc.
/// </summary>
/// <param name="hwnd">The HWND.</param>
/// <param name="message">The message.</param>
/// <param name="wParam">The w parameter.</param>
/// <param name="lParam">The l parameter.</param>
/// <returns></returns>
auto RubyGM::CGMFramework::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept->LRESULT {
    // 返回值
    LRESULT recode = 0;
    // 创建窗口时设置指针
    if (message == WM_CREATE) {
        // 获取指针
        auto* window = reinterpret_cast<RubyGM::CGMFramework*>(
            (reinterpret_cast<LPCREATESTRUCT>(lParam))->lpCreateParams
            );
        // 设置窗口指针
        ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, LONG_PTR(window));
        // TODO: 创建完毕
        //window->OnCreate(hwnd);
        // 返回1
        recode = 1;
    }
    else {
        // 获取储存的指针
        auto* window = reinterpret_cast<RubyGM::CGMFramework*>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(hwnd, GWLP_USERDATA))
            );
        // 无效
        if (!window) return ::DefWindowProcW(hwnd, message, wParam, lParam);
        // 未处理
        if (!window->MessageHandle(message, wParam, lParam, recode)) {
            recode = ::DefWindowProcW(hwnd, message, wParam, lParam);
        }
    }
    return recode;
}


/// <summary>
/// Renders this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMFramework::Render() noexcept -> HRESULT {
    // 重建FPS
    this->recreate_fps_layout();
    // 开始刻画
    m_pd2dDeviceContext->BeginDraw();
    {
        m_pd2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::RoyalBlue));
        // 渲染精灵
        for (auto& sprite : m_ltSprites) {
            sprite.Render(*reinterpret_cast<IGMRednerContext*>(m_pd2dDeviceContext));
        }
        // 渲染FPS
        if (m_pFPSLayout) {
            m_pd2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
            m_pd2dDeviceContext->DrawTextLayout(
                D2D1::Point2F(),
                m_pFPSLayout,
                m_pBaiscBrush
            );
        }
    }
    // 结束刻画
    auto hr = m_pd2dDeviceContext->EndDraw();
    assert(SUCCEEDED(hr));
    // 向交换链提交帧
    if (SUCCEEDED(hr)) {
        hr = m_pSwapChain->Present(0, 0);
    }
    assert(SUCCEEDED(hr));
    // 重建资源
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
        hr = this->Recreate();
    }
    assert(SUCCEEDED(hr));
    return S_OK;
}


bool RubyGM::CGMFramework::MessageHandle(
    UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result) noexcept {
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(result);
    // 检查信息
    switch (message)
    {
    case WM_CLOSE:
        ::DestroyWindow(m_hwnd);
        return false;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        result = 0;
        return true;
    }
    return false;
}

/// <summary>
/// Runs this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMFramework::Run() noexcept {
    ::ShowWindow(m_hwnd, SW_SHOW);
    auto update_fiber = [](void*) { RubyGM::UpdateFiber(); };
    auto fiber0 = ::ConvertThreadToFiberEx(nullptr, FIBER_FLAG_FLOAT_SWITCH);
    auto fiber1 = ::CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, update_fiber, fiber0);
    assert(m_hVBlank);
    MSG msg;
    // 循环
    while (true) {
        // 处理事件
        switch (::MsgWaitForMultipleObjects(1, &m_hVBlank, FALSE, INFINITE, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0:
            // 渲染对象
            ::SwitchToFiber(fiber1);
            this->Render();
            break;
        case WAIT_OBJECT_0 + 1:
            // 处理消息
            while (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                // 退出?
                if (msg.message == WM_QUIT) goto force_break;
                ::TranslateMessage(&msg);
                ::DispatchMessageW(&msg);
            }
            break;
        default:
            goto force_break;
        }
    }
    // 收尾处理
force_break:
    int bk; bk = 9;
    ::DeleteFiber(fiber1);
}

// 重建FPS布局
void RubyGM::CGMFramework::recreate_fps_layout() noexcept {
    assert(m_pTextFormatMain && "bad initialize");
    constexpr uint32_t FRAME_COUNT = 8;
    constexpr uint32_t BUFFER_COUNT = 64;
    // 增加计数
    ++m_uFrameCount;
    if (m_uFrameCount % FRAME_COUNT) return;
    // 计算
    wchar_t buffer[BUFFER_COUNT];
    auto now = impl::get_time();
    m_fps = 1000.f * static_cast<float>(FRAME_COUNT) / static_cast<float>(now - m_uLastFrameTime);
    m_uLastFrameTime = now;
    // 释放数据
    RubyGM::SafeRelease(m_pFPSLayout);
    // 格式化
    auto c = std::swprintf(
        buffer, BUFFER_COUNT,
        L"%6.2f @%ld",
        m_fps,
        long(m_uFrameCount)
    );
    assert(c > 0 && "bad std::swprintf call");
    // 生成文本布局
    m_pDWriteFactory->CreateTextLayout(
        buffer, static_cast<UINT32>(c),
        m_pTextFormatMain,
        float(m_uWndWidth), float(m_uWndHeight),
        &m_pFPSLayout
    );
}

/// <summary>
/// Adds the sprite.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMFramework::AddSprite() noexcept -> CGMSprite* {
    try {
        m_ltSprites.emplace_back(nullptr);
        return &m_ltSprites.back();
    }
    catch (...) {
        return nullptr;
    }
}


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

/// <summary>
///
/// </summary>




#pragma comment(lib, "libmruby")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Shcore")
#pragma comment(lib, "Winmm")
#pragma comment(lib, "dwrite")
