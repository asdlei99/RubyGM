#include <LongUI.h>
#include <bridge/rgmluiBridge.h>
#include <bridge/rgmluiConfig.h>

// get xml layout
auto GetRubyGMXmlLayout() noexcept -> const char*;

// Entry for App
int main(int argc, const char* argv[]) {
    // show
    int nCmdShow = SW_SHOWDEFAULT;
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
            UIManager.CreateUIWindow(GetRubyGMXmlLayout())->ShowWindow(nCmdShow);
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
<Window title="RubyGM - Test" textantimode="cleartype">
    <HorizontalLayout>
        <Single><Game/></Single>
        <VerticalLayout weight="0.2">
            <Null/>
            <Button text="Button" margin="4,4,4,4" borderwidth="1" size="0, 32"/>
            <Null/>
        </VerticalLayout>
    </HorizontalLayout>
</Window>
)xml)";

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
