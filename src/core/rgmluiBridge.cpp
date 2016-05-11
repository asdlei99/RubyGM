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
            <Button text="Button2" 
                margin="4,4,4,4" borderwidth="1" size="0, 32"/>
            <Button text="Button3" 
                margin="4,4,4,4" borderwidth="1" size="0, 32"/>
            <Null/>
        </VerticalLayout>
    </HorizontalLayout>
</Window>
)xml)";

// init window
void InitWindow(LongUI::XUIBaseWindow* window, int cmd) noexcept {
    if (window) {
        using LongUI::longui_cast;
        LongUI::UIControl* control = nullptr;;
        if (control = (window->FindControl("btnRecreate"))) {
            control->AddEventCall([](LongUI::UIControl*) {
                UIManager.RecreateResources();
                return true;
            }, LongUI::SubEvent::Event_ItemClicked);
        }
        window->ShowWindow(cmd);
    }
}

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

