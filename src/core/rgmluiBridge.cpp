#include <LongUI.h>
#include <bridge/rgmluiBridge.h>
#include <bridge/rgmluiConfig.h>

// get xml layout
auto GetRubyGMXmlLayout() noexcept -> const char*;
// init window
void InitWindow(LongUI::XUIBaseWindow* , int) noexcept;

// Entry for App
int WINAPI WinMain(HINSTANCE, HINSTANCE, char* lpCmdLine, int nCmdShow) noexcept {
    // every windows desktop app should do this
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    // use OleInitialize to init ole and com
    if (SUCCEEDED(::OleInitialize(nullptr))) {
        // config
        RubyGM::Bridge::Configure config;
        // init longui manager
        if (SUCCEEDED(UIManager.Initialize(&config))) {
            // my style
            UIManager << DL_Hint << L"Battle Control Online!" << LongUI::endl;
            // create main window, return nullptr for some error
            InitWindow(UIManager.CreateUIWindow(GetRubyGMXmlLayout()), nCmdShow);
            // run this app
            UIManager.Run();
            // my style
            UIManager << DL_Hint << L"Battle Control Terminated!" << LongUI::endl;
        }
        // cleanup longui
        UIManager.Uninitialize();
    }
    // cleanup ole and com
    ::OleUninitialize();
    // exit
    return EXIT_SUCCESS;
}

// xml layout
const char* const RUBYGM_XML = u8R"(xml(<?xml version="1.0" encoding="utf-8"?>
<Window titlename="RubyGM - Test" __textantimode="cleartype">
    <HorizontalLayout>
        <Single><Game/></Single>
        <VerticalLayout weight="0.2">
            <Null/>
            <Button name="btnRecreate" text="重建资源" 
                margin="4,4,4,4" borderwidth="1" size="0, 32"/>
            <Button name="btnFullDraw" text="完全刷新" 
                margin="4,4,4,4" borderwidth="1" size="0, 32"/>
            <Button name="btnPauseResume"   text="暂停恢复" 
                margin="4,4,4,4" borderwidth="1" size="0, 32"/>
            <Slider name="sldTimeScale"     value="1"
                margin="4,4,4,4" size="0, 32"/>
            <Null/>
        </VerticalLayout>
    </HorizontalLayout>
</Window>
)xml)";

#include <game/rgmGame.h>

// init window
void InitWindow(LongUI::XUIBaseWindow* window, int cmd) noexcept {
    if (window) {
        using LongUI::longui_cast;
        if (const auto control = (window->FindControl("btnRecreate"))) {
            control->AddEventCall([](LongUI::UIControl*) {
                UIManager.RecreateResources();
                return true;
            }, LongUI::SubEvent::Event_ItemClicked);
        }
        if (const auto control = (window->FindControl("btnFullDraw"))) {
            control->AddEventCall([window](LongUI::UIControl*) {
                window->InvalidateWindow();
                return true;
            }, LongUI::SubEvent::Event_ItemClicked);
        }
        if (const auto control = (window->FindControl("btnPauseResume"))) {
            const auto slider = window->FindControl("sldTimeScale");
            control->AddEventCall([slider](LongUI::UIControl*) {
                float ts = RubyGM::Game::GetTimeScale();
                ts = ts == 0.f ? 1.f : 0.f;
                slider->SetFloat(ts);
                RubyGM::Game::SetTimeScale(ts);
                return true;
            }, LongUI::SubEvent::Event_ItemClicked);
        }
        if (const auto control = (window->FindControl("sldTimeScale"))) {
            control->AddEventCall([](LongUI::UIControl* sld) {
                RubyGM::Game::SetTimeScale(sld->GetFloat());
                return true;
            }, LongUI::SubEvent::Event_ValueChanged);
        }
        window->ShowWindow(cmd);
    }
}


#include <core/graphics/rgmGraphics.h>

// rubygm namespace
namespace RubyGM {
    // effect
    struct IGMEffect;
}

// rubygm::bridge namespace
namespace RubyGM { namespace Bridge {
    // create path geo
    auto CreatePathGeometry(IGMPath*& geo) noexcept -> Result {
#if 0
        using geotype = typename IGMPath::Super;
        using same = std::is_same<ID2D1PathGeometry, geotype>;
        static_assert(same::value, "must be same");
#endif
        auto** path = reinterpret_cast<ID2D1PathGeometry**>(&geo);
        return Result(UIManager_D2DFactory->CreatePathGeometry(path));
    }
    // create effect from guid
    auto EffectFrom(const GUID& id) noexcept->IGMEffect* {
        ID2D1Effect* effect = nullptr;
        UIManager_RenderTarget->CreateEffect(id, &effect);
        return reinterpret_cast<IGMEffect*>(effect);
    }
}}


// message box
int msgbox_error(const char* str) {
    return ::MessageBoxA(nullptr, str, "RubyGM - Error", MB_ICONERROR);
}

// xml layout
auto GetRubyGMXmlLayout() noexcept -> const char* { return RUBYGM_XML; }

#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Shcore")
#pragma comment(lib, "Winmm")
#pragma comment(lib, "dwrite")

#pragma comment(lib, "longui")
#pragma comment(lib, "dlmalloc")
#pragma comment(lib, "pugixml")
#pragma comment(lib, "libmruby")


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

