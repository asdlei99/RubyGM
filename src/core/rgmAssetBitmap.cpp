#define _WIN32_WINNT 0x0A000001
#include <core/brush/rgmBrushStruct.h>
#include <core/asset/rgmAsset.h>
#include <core/util/rgmUtil.h>
#include <core/graphics/rgmGraphics.h>
#include <game/rgmGame.h>
#include <memory>
#include <algorithm>
#undef max
#undef min

// ============================================================================
// ============================ FileBitmap ====================================
// ============================================================================

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    // get null bitma for oom or some error
    auto GetNullBitmapAsset() noexcept->Bitmap&;
    // bitmap form file
    class FileBitmap final : public Bitmap {
    public:
        // create one instance
        static auto Create(const wchar_t*, const wchar_t*) noexcept ->FileBitmap* ;
        // get bitmap
        auto GetBitmap() noexcept -> IGMBitmap* override;
        // low occu
        void LowOccupancy() noexcept override;
    protected:
        // recreate resource
        virtual auto recreate() noexcept -> Result override;
    private:
        // ctor
        FileBitmap( const wchar_t* , const wchar_t* ) noexcept;
        // ctor
        ~FileBitmap() noexcept;
        // dispose
        void dispose() noexcept override;
    private:
        // bitmap object
        IGMBitmap*          m_pBitmap = nullptr;
        // file name
        wchar_t             m_szFileName[0];
    };
}}

/// <summary>
/// Creates the bitmap asset from file.
/// </summary>
/// <param name="namebgn">The namebgn.</param>
/// <param name="nameend">The nameend.</param>
/// <returns></returns>
auto RubyGM::Game::CreateBitmapAssetFromFile(
    const wchar_t * namebgn, 
    const wchar_t * nameend) noexcept -> Asset::Bitmap& {
    // 返回创建的对象
    if (auto ptr = Asset::FileBitmap::Create(namebgn, nameend)) {
        return *ptr;
    }
    // 返回空资源对象
    else {
        return Asset::GetNullBitmapAsset();
    }
}

/// <summary>
/// Creates the specified .
/// </summary>
/// <param name="">The .</param>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Asset::FileBitmap::Create(
    const wchar_t* begin, 
    const wchar_t* end) noexcept -> FileBitmap * {
    assert(begin && end >= begin);
    // 路径长度是动态的, 所以用正常的分配函数
    const size_t len = ((end - begin) + 1) * sizeof(wchar_t);
    // 分配指定内存
    if (const auto ptr = RubyGM::NormalAlloc(sizeof(FileBitmap) + len)) {
        return new(ptr) FileBitmap(begin, end);
    }
    else {
        return nullptr;
    }
}

/// <summary>
/// Initializes a new instance of the <see cref="FileBitmap"/> class.
/// </summary>
/// <param name="begin">The begin.</param>
/// <param name="end">The end.</param>
RubyGM::Asset::FileBitmap::FileBitmap(
     const wchar_t* begin, const wchar_t* end) noexcept {
    // 复制字符串
    std::memcpy(m_szFileName, begin, (end - begin) * sizeof(wchar_t));
    m_szFileName[end - begin] = 0;
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Asset::FileBitmap::recreate() noexcept -> Result {
    // 释放资源, 等待下一次重建
    RubyGM::SafeRelease(m_pBitmap);
    return Result(S_OK);
}

/// <summary>
/// Lows the occupancy.
/// </summary>
/// <returns></returns>
void RubyGM::Asset::FileBitmap::LowOccupancy() noexcept {
    this->LowOccupancyHelper(m_pBitmap);
}

/// <summary>
/// Finalizes an instance of the <see cref="FileBitmap"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Asset::FileBitmap::~FileBitmap() noexcept {
    RubyGM::SafeRelease(m_pBitmap);
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Asset::FileBitmap::dispose() noexcept {
    this->FileBitmap::~FileBitmap();
    RubyGM::NormalFree(this);
}


// rubygm::dx namespace
namespace RubyGM { namespace DX {
    // create bitmap from file
    auto CreateBitmapFromFile(const wchar_t* url, 
        IGMBitmap*& bitmap) noexcept->Result;
    // compute normal scale factor
    auto ComputeMidScaleFactor(const Matrix3X2F& m) noexcept -> float {
        // 简单的缩放
        if (m._12 == 0.0f && m._21 == 0.0f)  {
            return (std::abs(m._11) + std::abs(m._22)) * 0.5f;
        }
        // 复杂的转变
        else {
            constexpr float sqrt2 = 1.41421356237f;
            float a = m._11 + m._21;
            float b = m._12 + m._22;
            float c = std::sqrt(a*a + b*b) / sqrt2;
            return c;
            /*float a = m._11 * m._11 + m._12 * m._12;
            float b = m._11 * m._21 + m._12 * m._22;
            float c = m._21 * m._21 + m._22 * m._22;
            float d = a - c;
            float r = std::sqrt(d * d + b * b * 4);
            return std::sqrtf((a + c + r) * 0.5f);*/
        }
    }
}}

/// <summary>
/// Gets the bitmap.
/// </summary>
/// <returns></returns>
auto RubyGM::Asset::FileBitmap::GetBitmap() noexcept -> IGMBitmap* {
    // 没有就创建
    if (!m_pBitmap) {
        auto hr = DX::CreateBitmapFromFile(m_szFileName, m_pBitmap);
        if (FAILED(hr)) Game::SetLastErrorCode(hr);
    }
    // 增加引用
    return RubyGM::SafeAcquire(m_pBitmap);
}

// ============================================================================
// =========================== RasterBitmap ===================================
// ============================================================================
#include <core/drawable/rgmDrawable.h>
#include <core/graphics/rgmSprite.h>

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    // implement for RasterBitmapImpl
    class RasterBitmapImpl final : public RasterBitmap {
    public:
        // create one instance
        static auto Create(
            Drawable::Object* obj,
            RubyGM::SizeF zoom,
            RubyGM::SizeF size
        ) noexcept ->RasterBitmapImpl* ;
        // get bitmap
        auto GetBitmap() noexcept -> IGMBitmap* override;
        // redraw
        auto Redraw() noexcept ->Result override;
        // low occu, do nothing for this class
        void LowOccupancy() noexcept override {  }
    protected:
        // recreate resource
        virtual auto recreate() noexcept -> Result override;
    private:
        // ctor
        RasterBitmapImpl(
            Drawable::Object& obj,
            RubyGM::SizeF zoom,
            RubyGM::SizeF size
        ) noexcept;
        // ctor
        ~RasterBitmapImpl() noexcept;
        // dispose
        void dispose() noexcept override;
    private:
        // drawable object
        Drawable::Object&           m_refDrawble;
        // bitmap
        IGMBitmap*                  m_pBitmap = nullptr;
        // bitmap render target
        IGMRenderContext*           m_pRenderTarget = nullptr;
        // zoom factor
        RubyGM::SizeF               m_szZoom;
        // basic size of bitmap
        RubyGM::SizeF               m_szBitmap;
    };
}}


/// <summary>
/// Rasters the bitmap.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
RubyGM::Asset::RasterBitmapImpl::RasterBitmapImpl(
    Drawable::Object& obj,
    RubyGM::SizeF zoom,
    RubyGM::SizeF size) noexcept :
m_refDrawble(obj),
m_szZoom(zoom),
m_szBitmap(size) {
    m_refDrawble.AddRef();
}


/// <summary>
/// Finalizes an instance of the <see cref="RasterBitmapImpl"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Asset::RasterBitmapImpl::~RasterBitmapImpl() noexcept {
    m_refDrawble.Release();
    RubyGM::SafeRelease(m_pBitmap);
    RubyGM::SafeRelease(m_pRenderTarget);
}


// rubygm::impl namespace
namespace RubyGM { namespace impl {
    // create bitmap render target
    auto create_bitmap_render_target(
        RubyGM::SizeF size, ID2D1BitmapRenderTarget** brt
    ) noexcept->Result;
    // get iid to rc
    auto get_rendercontext_iid() noexcept -> const IID&;
    // get iid to bmp
    auto get_bitmap_iid() noexcept -> const IID&;
    // get root transform
    auto get_root_transform() noexcept -> const Matrix3X2F&;
}}


/// <summary>
/// Redraws this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Asset::RasterBitmapImpl::Redraw() noexcept ->Result {
    Result hr = Result(S_OK);
    // 没有就创建
    if (!m_pRenderTarget) {
        // 创建位图呈现器
        ID2D1BitmapRenderTarget* brt = nullptr;
        ID2D1Bitmap* bmp = nullptr;
        {
            float a = m_szZoom.width, b = m_szZoom.height;
            SizeF ts{ m_szBitmap.width * a, m_szBitmap.height*  b };
            hr = impl::create_bitmap_render_target(ts, &brt);
        }
        // 获取设备上下文
        if (SUCCEEDED(hr)) {
            hr = Result(brt->QueryInterface(
                impl::get_rendercontext_iid(), 
                reinterpret_cast<void**>(&m_pRenderTarget)
            ));
        }
        // 获取呈现目标
        if (SUCCEEDED(hr)) {
            hr = Result(brt->GetBitmap(&bmp));
        }
        // 获取位图
        if (SUCCEEDED(hr)) {
            hr = Result(bmp->QueryInterface(
                impl::get_bitmap_iid(), 
                reinterpret_cast<void**>(&m_pBitmap)
            ));
        }
        // 扫尾处理
        RubyGM::SafeRelease(brt);
        RubyGM::SafeRelease(bmp);
    }
    // 渲染目标
    if (SUCCEEDED(hr)) {
        {
            float a = m_szZoom.width, b = m_szZoom.height;
            D2D1_MATRIX_3X2_F m;
            m._11 = a; m._12 = 0;
            m._21 = 0; m._22 = b;
            m._31 = 0; m._32 = 0;
            m_pRenderTarget->SetTransform(&m);
        }
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->Clear(nullptr);
        m_refDrawble.Render(*m_pRenderTarget);
        hr = Result(m_pRenderTarget->EndDraw());
    }
    // 返回
    return hr;
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Asset::RasterBitmapImpl::recreate() noexcept -> Result {
    // 释放资源, 等待下一次重建
    RubyGM::SafeRelease(m_pBitmap);
    RubyGM::SafeRelease(m_pRenderTarget);
    return Result(S_OK);
}


/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Asset::RasterBitmapImpl::dispose() noexcept {
    this->RasterBitmapImpl::~RasterBitmapImpl();
    RubyGM::SmallFree(this);
}

/// <summary>
/// Gets the bitmap.
/// </summary>
/// <returns></returns>
auto RubyGM::Asset::RasterBitmapImpl::GetBitmap() noexcept -> IGMBitmap * {
    // 无位图
    if (!m_pBitmap) {
        // 创建
        auto hr = this->Redraw();
        // 失败
        if (FAILED(hr)) Game::SetLastErrorCode(hr);
    }
    // 增加引用
    return RubyGM::SafeAcquire(m_pBitmap);
}

/// <summary>
/// Creates the specified object.
/// </summary>
/// <param name="obj">The object.</param>
/// <returns></returns>
auto RubyGM::Asset::RasterBitmapImpl::Create(
    Drawable::Object* obj, 
    RubyGM::SizeF zoom,
    RubyGM::SizeF size) noexcept -> RasterBitmapImpl* {
    // 无效对象
    if (obj) {
        // 申请失败
        if (const auto ptr = RubyGM::SmallAlloc(sizeof(RasterBitmapImpl))) {
            return new(ptr) RasterBitmapImpl(*obj, zoom, size);
        }
    }
    return nullptr;
}

/// <summary>
/// Creates the bitmap asset from drawable.
/// </summary>
/// <param name="ob">The drawable object.</param>
/// <param name="sf">The scale factor.</param>
/// <param name="bs">The basic size.</param>
/// <returns></returns>
auto RubyGM::Game::CreateBitmapAssetFromDrawable(
    Drawable::Object* ob, SizeF sf, SizeF bs) noexcept -> Asset::Bitmap& {
    // 获取缩放后位图大小
    const SizeF s{ bs.width*sf.width, bs.height*sf.height };
    const bool wok = s.width >= 4.f && s.width < 4096.f;
    const bool hok = s.height >= 4.f && s.height < 4096.f;
    // 检查大小合法性
    if (wok && hok) {
        // 创建对象
        if (auto ptr = Asset::RasterBitmapImpl::Create(ob, sf, bs)) {
            return *ptr;
        }
    }
    // 返回空对象
    return Asset::GetNullBitmapAsset();
}
