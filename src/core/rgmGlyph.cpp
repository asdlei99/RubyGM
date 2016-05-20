#define _WIN32_WINNT 0x0A000001
#include <core/rubygm.h>
#include <core/drawable/rgmDrawable.h>
#include <core/graphics/rgmGraphics.h>
#include <core/drawable/rgmGlyph.h>
#include <core/Text/rgmTextDetail.h>
#include <core/drawable/rgmTextlayout.h>
//#include <bridge/rgmluiBridge.h>
//#include <core/util/rgmImpl.h>
//#include <core/asset/rgmAssetFont.h>
//#include <game/rgmGame.h>


// rubygm namespace
namespace RubyGM {
    // bridge
    namespace Bridge {
        // create path
        auto CreatePathGeometry(IGMGeometry*& geo) noexcept->Result;
    }
    // recorder
    class CGMGlyphRecorder final : public IDWriteTextRenderer {
    public:
        // quert interface
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID&, void**) noexcept;
        // add ref-count
        virtual ULONG   STDMETHODCALLTYPE AddRef() override { return 2; }
        // relesse
        virtual ULONG   STDMETHODCALLTYPE Release() override { return 1; }
    public:
        // is pixel snapping disabled?
        virtual HRESULT STDMETHODCALLTYPE IsPixelSnappingDisabled(void*, BOOL*) noexcept override;
        // get current transform
        virtual HRESULT STDMETHODCALLTYPE GetCurrentTransform(void*, DWRITE_MATRIX*) noexcept override;
        // get bilibili of px/pt
        virtual HRESULT STDMETHODCALLTYPE GetPixelsPerDip(void*, FLOAT*) noexcept override;
    public:
        // draw glyphrun
        virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(
            void* clientDrawingContext,
            FLOAT baselineOriginX,
            FLOAT baselineOriginY,
            DWRITE_MEASURING_MODE measuringMode,
            const DWRITE_GLYPH_RUN* glyphRun,
            const DWRITE_GLYPH_RUN_DESCRIPTION* glyphRunDescription,
            IUnknown* clientDrawingEffect
        ) noexcept override;
        // draw inline object implemented as template
        virtual HRESULT STDMETHODCALLTYPE DrawInlineObject(
            _In_opt_ void* clientDrawingContext,
            FLOAT originX,
            FLOAT originY,
            _In_ IDWriteInlineObject* inlineObject,
            BOOL isSideways,
            BOOL isRightToLeft,
            _In_opt_ IUnknown* clientDrawingEffect
        ) noexcept override;
        // draw underline
        virtual HRESULT STDMETHODCALLTYPE DrawUnderline(
            void* clientDrawingContext,
            FLOAT baselineOriginX,
            FLOAT baselineOriginY,
            const DWRITE_UNDERLINE* underline,
            IUnknown* clientDrawingEffect
        ) noexcept override;
        // draw strikethrough
        virtual HRESULT STDMETHODCALLTYPE DrawStrikethrough(
            void* clientDrawingContext,
            FLOAT baselineOriginX,
            FLOAT baselineOriginY,
            const DWRITE_STRIKETHROUGH* strikethrough,
            IUnknown* clientDrawingEffect
        ) noexcept override;
    public:
        // sink
        ID2D1GeometrySink*      sink = nullptr;
    private:
    };
}


/// <summary>
/// Initializes a new instance of the <see cref="Glyph"/> class.
/// </summary>
/// <param name="bs">The bs.</param>
RubyGM::Drawable::Glyph::Glyph(const GlyphStatus& gs) noexcept : Super(gs) {
    auto hr = Bridge::CreatePathGeometry(m_pTextGlyph);
    assert(gs.layout);
    auto layout = gs.layout->m_pTextlayout;
    ID2D1GeometrySink* sink = nullptr;
    if (SUCCEEDED(hr)) {
        hr = m_pTextGlyph->Open(&sink);
    }
    if (SUCCEEDED(hr)) {
        CGMGlyphRecorder recorder;
        recorder.sink = sink;
        auto thr = layout->Draw(nullptr, &recorder, 0.f, 0.f);
        hr = sink->Close();
    }
    auto hhrr = HRESULT(hr);
    assert(SUCCEEDED(hr));
    RubyGM::SafeRelease(sink);
}


/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Glyph::Render(IGMRenderContext& rc) const noexcept {
    rc.DrawGeometry(m_pTextGlyph, m_pGiBrushStroke, 0.2f);
}

/// <summary>
/// Creates the specified .
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Drawable::Glyph::Create(const GlyphStatus& bs) noexcept ->Glyph* {
    if (const auto ptr = RubyGM::SmallAlloc(sizeof(Glyph))) {
        return new(ptr) Glyph(bs);
    }
    return nullptr;
}

/// <summary>
/// Finalizes an instance of the <see cref="Glyph"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Glyph::~Glyph() noexcept {
    RubyGM::SafeRelease(m_pTextGlyph);
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
//auto RubyGM::Drawable::Glyph::recreate() noexcept -> Result {
   // return Super::recreate();
//}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Glyph::dispose() noexcept {
    this->Glyph::~Glyph();
    RubyGM::SmallFree(this);
}



// ============================================================================
// ========================= CGMGlyphRecorder =================================
// ============================================================================

// longui namespace
namespace LongUI {
    // iid for IDWriteTextRenderer
    extern const IID IID_IDWriteTextRenderer;
}

/// <summary>
/// Queries the interface.
/// </summary>
/// <param name="">The .</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::QueryInterface(
    const IID& iid, void ** ptr) noexcept -> HRESULT {
    // iid for unkown
    if (iid == IID_IUnknown) {
        *ptr = static_cast<IUnknown*>(this);
        return S_OK;
    }
    // iid for IDWriteTextRenderer
    else if(iid == LongUI::IID_IDWriteTextRenderer) {
        *ptr = static_cast<IDWriteTextRenderer*>(this);
        return S_OK;
    }
    // no interface
    return E_NOINTERFACE;
}

/// <summary>
/// Determines whether [is pixel snapping disabled] [the specified ].
/// </summary>
/// <param name="">The .</param>
/// <param name="isDisabled">The is disabled.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::IsPixelSnappingDisabled(
    void*, BOOL * isDisabled) noexcept ->HRESULT {
    *isDisabled = false;
    return S_OK;
}

/// <summary>
/// Gets the current transform.
/// </summary>
/// <param name="">The .</param>
/// <param name="mat">The mat.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::GetCurrentTransform(
    void*, DWRITE_MATRIX* mat) noexcept -> HRESULT{
    mat->m11 = 1.f; mat->m12 = 0.f;
    mat->m11 = 0.f; mat->m22 = 1.f;
    mat->dx  = 0.f; mat->dy  = 0.f;
    return S_OK;
}

/// <summary>
/// Gets the pixels per dip.
/// </summary>
/// <param name="">The .</param>
/// <param name="bilibili">The bilibili.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::GetPixelsPerDip(
    void*, FLOAT * bilibili) noexcept -> HRESULT {
    *bilibili = 1.f;
    return S_OK;
}


/// <summary>
/// Draws the inline object.
/// </summary>
/// <param name="clientDrawingContext">The client drawing context.</param>
/// <param name="originX">The origin x.</param>
/// <param name="originY">The origin y.</param>
/// <param name="inlineObject">The inline object.</param>
/// <param name="isSideways">The is sideways.</param>
/// <param name="isRightToLeft">The is right to left.</param>
/// <param name="clientDrawingEffect">The client drawing effect.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::DrawInlineObject(
    _In_opt_ void* clientDrawingContext,
    FLOAT originX, FLOAT originY,
    _In_ IDWriteInlineObject * inlineObject,
    BOOL isSideways, BOOL isRightToLeft,
    _In_opt_ IUnknown* clientDrawingEffect) noexcept -> HRESULT {
    UNREFERENCED_PARAMETER(isSideways);
    UNREFERENCED_PARAMETER(isRightToLeft);
    assert(inlineObject && "bad argument");
    // 渲染
    inlineObject->Draw(
        clientDrawingContext, this,
        originX, originY,
        isSideways, isRightToLeft,
        clientDrawingEffect
    );
    return S_OK;
}


/// <summary>
/// Draws the underline.
/// </summary>
/// <param name="clientDrawingContext">The client drawing context.</param>
/// <param name="baselineOriginX">The baseline origin x.</param>
/// <param name="baselineOriginY">The baseline origin y.</param>
/// <param name="underline">The underline.</param>
/// <param name="effect">The effect.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::DrawUnderline(
    void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    const DWRITE_UNDERLINE* underline,
    IUnknown* effect) noexcept -> HRESULT{
    UNREFERENCED_PARAMETER(clientDrawingContext);
    // 计算矩形
    D2D1_RECT_F rectangle = {
        baselineOriginX,
        baselineOriginY + underline->offset,
        baselineOriginX + underline->width,
        baselineOriginY + underline->offset + underline->thickness
    };
    return S_OK;
}


/// <summary>
/// Draws the strikethrough.
/// </summary>
/// <param name="clientDrawingContext">The client drawing context.</param>
/// <param name="baselineOriginX">The baseline origin x.</param>
/// <param name="baselineOriginY">The baseline origin y.</param>
/// <param name="strikethrough">The strikethrough.</param>
/// <param name="effect">The effect.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::DrawStrikethrough(
    void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    const DWRITE_STRIKETHROUGH* strikethrough,
    IUnknown* effect) noexcept ->HRESULT {
    // 计算矩形
    D2D1_RECT_F rectangle = {
        baselineOriginX,
        baselineOriginY + strikethrough->offset,
        baselineOriginX + strikethrough->width,
        baselineOriginY + strikethrough->offset + strikethrough->thickness
    };
    return S_OK;
}


/// <summary>
/// Draws the glyph run.
/// </summary>
/// <param name="clientDrawingContext">The client drawing context.</param>
/// <param name="baselineOriginX">The baseline origin x.</param>
/// <param name="baselineOriginY">The baseline origin y.</param>
/// <param name="measuringMode">The measuring mode.</param>
/// <param name="glyphRun">The glyph run.</param>
/// <param name="glyphRunDescription">The glyph run description.</param>
/// <param name="effect">The effect.</param>
/// <returns></returns>
auto RubyGM::CGMGlyphRecorder::DrawGlyphRun(
    void* clientDrawingContext,
    FLOAT baselineOriginX, FLOAT baselineOriginY,
    DWRITE_MEASURING_MODE measuringMode,
    const DWRITE_GLYPH_RUN * glyphRun,
    const DWRITE_GLYPH_RUN_DESCRIPTION * glyphRunDescription,
    IUnknown * effect) noexcept -> HRESULT {
    UNREFERENCED_PARAMETER(clientDrawingContext);
    UNREFERENCED_PARAMETER(glyphRunDescription);
#ifdef _DEBUG
    constexpr uint32_t BUFLEN = 2;
#else
    constexpr uint32_t BUFLEN = 256;
#endif
    DWRITE_GLYPH_OFFSET buffer[BUFLEN];
    DWRITE_GLYPH_OFFSET* buf = buffer;
    const auto c = glyphRun->glyphCount;
    if (c > BUFLEN) {
        auto ptr = RubyGM::NormalAlloc(sizeof(DWRITE_GLYPH_OFFSET) * c);
        buf = reinterpret_cast<DWRITE_GLYPH_OFFSET*>(ptr);
    }
    if (!buf) return E_OUTOFMEMORY;
    // XXX: 进行手动偏移 -- 没有其他办法了?
    const auto offsets = glyphRun->glyphOffsets;
    for (uint32_t i = 0; i < c; ++i) {
        buf[i] = {
            offsets[i].advanceOffset + baselineOriginX,
            offsets[i].ascenderOffset + baselineOriginY
        };
    }
    // 获取轮廓几何
    auto hr = glyphRun->fontFace->GetGlyphRunOutline(
        glyphRun->fontEmSize,
        glyphRun->glyphIndices,
        glyphRun->glyphAdvances,
        buf,
        glyphRun->glyphCount,
        glyphRun->isSideways,
        (glyphRun->bidiLevel & 1),
        this->sink
    );
    if (buf != buffer) {
        RubyGM::NormalFree(buf);
    }
    return hr;
}