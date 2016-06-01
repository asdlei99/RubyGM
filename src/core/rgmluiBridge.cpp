#define _WIN32_WINNT 0x0A000001
#include <LongUI.h>
#include <Graphics/luiGrSvg.h>
#include <bridge/rgmluiBridge.h>
#include <bridge/rgmluiConfig.h>

// get xml layout
/*auto GetRubyGMXmlLayout() noexcept -> const char*;
// init window
void InitWindow(LongUI::XUIBaseWindow* , int) noexcept;
#ifdef _DEBUG
namespace RubyGM { namespace impl { void init(); void uninit(); } }
#endif
// init
inline void rubygm_init_call() {
#ifdef _DEBUG
    RubyGM::impl::init();
#endif
}
void rubygm_uninit_call() {
#ifdef _DEBUG
    RubyGM::impl::uninit();
#endif
}

// Entry for App
int WINAPI WinMain(HINSTANCE, HINSTANCE, char* lpCmdLine, int nCmdShow) noexcept {
    // every windows desktop app should do this
    ::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    // init
    rubygm_init_call();
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
    // un
    rubygm_uninit_call();
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
}*/


#include <core/graphics/rgmGraphics.h>

// rubygm namespace
namespace RubyGM {
    // effect
    struct IGMEffect;
}

// rubygm::bridge namespace
namespace RubyGM { namespace Bridge {
    /// <summary>
    /// Creates the path geometry.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns></returns>
    auto CreatePathGeometry(ID2D1PathGeometry*& path) noexcept -> Result {
        return Result(UIManager_D2DFactory->CreatePathGeometry(&path));
    }
    /// <summary>
    /// SVG the path geometry.
    /// </summary>
    /// <param name="str">The string.</param>
    /// <param name="path">The path.</param>
    /// <returns></returns>
    auto SvgPathGeometry(const char* str, 
        ID2D1PathGeometry1*& path) noexcept->Result {
        return Result(LongUI::SVG::ParserPath(str, &path));
    }
    /// <summary>
    /// Creates the filled geometry realization.
    /// </summary>
    /// <param name="geometry">The geometry.</param>
    /// <param name="flatteningTolerance">The flattening tolerance.</param>
    /// <param name="geometryRealization">The geometry realization.</param>
    /// <returns></returns>
    auto CreateFilledGeometryRealization(
        ID2D1Geometry* geometry,
        float flatteningTolerance,
        ID2D1GeometryRealization **geometryRealization 
    ) noexcept -> Result {
        assert(geometry && geometryRealization && flatteningTolerance > 0.f);
        return Result(UIManager_RenderTarget->CreateFilledGeometryRealization(
            geometry, 
            flatteningTolerance, 
            geometryRealization
        ));
    }
    /// <summary>
    /// Creates the stroked geometry realization.
    /// </summary>
    /// <param name="geometry">The geometry.</param>
    /// <param name="flatteningTolerance">The flattening tolerance.</param>
    /// <param name="strokeWidth">Width of the stroke.</param>
    /// <param name="strokeStyle">The stroke style.</param>
    /// <param name="geometryRealization">The geometry realization.</param>
    /// <returns></returns>
    auto CreateStrokedGeometryRealization(
        ID2D1Geometry *geometry,
        FLOAT flatteningTolerance,
        FLOAT strokeWidth,
        ID2D1StrokeStyle *strokeStyle,
        ID2D1GeometryRealization **geometryRealization 
    ) noexcept -> Result {
        assert(geometry && geometryRealization );
        assert(strokeWidth > 0.f && flatteningTolerance > 0.f);
        return Result(UIManager_RenderTarget->CreateStrokedGeometryRealization(
            geometry, 
            flatteningTolerance, 
            strokeWidth,
            strokeStyle,
            geometryRealization
        ));
    }
    // create effect from guid
    auto EffectFrom(const GUID& id) noexcept->IGMEffect* {
        ID2D1Effect* effect = nullptr;
        UIManager_RenderTarget->CreateEffect(id, &effect);
        return reinterpret_cast<IGMEffect*>(effect);
    }
}}


// message box
/*int msgbox_error(const char* str) {
    return ::MessageBoxA(nullptr, str, "RubyGM - Error", MB_ICONERROR);
}

// xml layout
auto GetRubyGMXmlLayout() noexcept -> const char* { return RUBYGM_XML; }*/


