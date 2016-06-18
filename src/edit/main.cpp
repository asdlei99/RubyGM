#define _WIN32_WINNT 0x0A000001
#pragma warning(disable: 4290)
#pragma warning(disable: 4200)
#include <LongUI.h>
#include <bridge/rgmluiConfig.h>
#include <edit/rgmMainView.h>

// window layout
static const char* const RUBYGM_EDITOR_LAYOUT 
= u8R"xml(
xml(<?xml version="1.0" encoding="utf-8"?>
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
)xml";


// Entry for App
int WINAPI WinMain(HINSTANCE, HINSTANCE, char* lpCmdLine, int nCmdShow) noexcept {
    // every windows desktop app should do this
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    // use OleInitialize to init ole and com
    if (SUCCEEDED(::OleInitialize(nullptr))) {
        // main viewport
        using RubyGM::Editor::MainViewport;
        // config
        RubyGM::Bridge::Configure config;
        // init longui manager
        if (SUCCEEDED(UIManager.Initialize(&config))) {
            // my style
            UIManager << DL_Hint << L"Battle Control Online!" << LongUI::endl;
            // create main window, return nullptr for some error
            UIManager.CreateUIWindow<MainViewport>(RUBYGM_EDITOR_LAYOUT);
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



#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Shcore")
#pragma comment(lib, "Winmm")
#pragma comment(lib, "dwrite")

#pragma comment(lib, "longui")
#pragma comment(lib, "dlmalloc")
#pragma comment(lib, "pugixml")
#pragma comment(lib, "rubygm-core")


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
