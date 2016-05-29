#define _WIN32_WINNT 0x0A000001
#include <core/util/rgmBrushStruct.h>
#include <core/util/rgmUtil.h>
#include <core/util/rgmImpl.h>
#include <core/asset/rgmAsset.h>
#include <core/graphics/rgmGraphics.h>
#include <bridge/rgmluiBridge.h>
#include <game/rgmGame.h>
#include <memory>


// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    // brush implement
    class BrushImpl : public Brush {
        // super class
        using Super = Brush;
    protected:
        // Recreates this instance.
        auto recreate() noexcept->Result override;
    public:
        // Lows the occupancy.
        void LowOccupancy() noexcept override final;
    public:
        // set opacity
        void SetOpacity(float opacity) noexcept override final;
        // set transfrom
        void SetTransfrom(const Matrix3X2F&) noexcept override final;
        // get opacity
        auto GetOpacity() const noexcept -> float override final {
            return m_fOpacity;
        }
        // get transfrom
        void GetTransfrom(Matrix3X2F& m) const noexcept override final {
            m = m_mTransform;
        }
    protected:
        // ctor
        BrushImpl() noexcept : Super() {}
        // dtor
        ~BrushImpl() noexcept { RubyGM::SafeRelease(m_pBrush); }
        // replace brush
        inline void repleace_brush(IGMBrush*&& brush) noexcept {
            RubyGM::SafeRelease(m_pBrush);
            if ((m_pBrush = brush)) {
                m_pBrush->SetOpacity(m_fOpacity);
                m_pBrush->SetTransform(&impl::d2d(m_mTransform));
            }
            brush = nullptr;
        }
    protected:
        // brush for this
        IGMBrush*           m_pBrush = nullptr;
        // opacity 
        float               m_fOpacity = 1.f;
        // unused float
        uint32_t            m_unused_brush = 233;
        // matrix
        Matrix3X2F          m_mTransform{ 1,0,0,1,0,0 };
    };
    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    auto BrushImpl::recreate() noexcept -> Result {
        // 释放资源, 等待下一次重建
        RubyGM::SafeRelease(m_pBrush);
        return Result(S_OK);
    }
    /// <summary>
    /// Lows the occupancy.
    /// </summary>
    /// <returns></returns>
    void BrushImpl::LowOccupancy() noexcept {
        this->LowOccupancyHelper(m_pBrush);
    }
    /// <summary>
    /// Sets the opacity.
    /// </summary>
    /// <param name="opacity">The opacity.</param>
    /// <returns></returns>
    void BrushImpl::SetOpacity(float opacity) noexcept {
        m_fOpacity = opacity;
        if (m_pBrush) m_pBrush->SetOpacity(opacity);
    }
    /// <summary>
    /// Sets the transfrom.
    /// </summary>
    /// <param name="">The .</param>
    /// <returns></returns>
    void BrushImpl::SetTransfrom(const Matrix3X2F& m) noexcept {
        m_mTransform = m;
        if (m_pBrush) m_pBrush->SetTransform(&impl::d2d(m));
    }
    // solid color brush asset
    class ColorBrush final : public BrushImpl {
    public:
        // create one instance
        static auto Create(const ColorF& prop) noexcept->ColorBrush*;
        // get brush
        auto GetBrush() noexcept -> IGMBrush* override;
    private:
        // ctor
        ColorBrush(const ColorF&) noexcept;
        // ctor
        ~ColorBrush() noexcept {}
        // dispose
        void dispose() noexcept override;
    private:
        // color properties
        ColorF                  m_color;
    };
    // linear gradient brush
    class LinearGradientBrush final : public BrushImpl {
    public:
        // create one instance
        static auto Create(const LinearBrush& prop) 
            noexcept->LinearGradientBrush*;
        // get brush
        auto GetBrush() noexcept -> IGMBrush* override;
    private:
        // ctor
        LinearGradientBrush(const LinearBrush&) noexcept;
        // ctor
        ~LinearGradientBrush() noexcept {}
        // dispose
        void dispose() noexcept override;
    private:
        // begin point
        Point2F             m_ptBegin;
        // end point
        Point2F             m_ptEnd;
        // gradient stops
        GradientStop        m_aStops[0];
    };
    // radial gradient brush
    class RadialGradientBrush final : public BrushImpl {
    public:
        // create one instance
        static auto Create(const RadialBrush& prop) 
            noexcept->RadialGradientBrush*;
        // get brush
        auto GetBrush() noexcept -> IGMBrush* override;
    private:
        // ctor
        RadialGradientBrush(const RadialBrush&) noexcept;
        // ctor
        ~RadialGradientBrush() noexcept {}
        // dispose
        void dispose() noexcept override;
    private:
        // begin point
        Point2F             m_ptCenter;
        // end point
        Point2F             m_ptOffset;
        // radius x
        float               m_fRadiusX;
        // radius y
        float               m_fRadiusY;
        // gradient stops
        GradientStop        m_aStops[0];
    };
    // bitmap brush
    class BitmapBrush final : public BrushImpl {
        // super class
        using Super = BrushImpl;
    public:
        // create one instance
        static auto Create(RefPtr<Asset::Bitmap>&&)
            noexcept->BitmapBrush*;
        // get brush
        auto GetBrush() noexcept -> IGMBrush* override;
    protected:
        // create
        auto recreate() noexcept->Result;
    private:
        // ctor
        BitmapBrush(RefPtr<Asset::Bitmap>&&) noexcept;
        // ctor
        ~BitmapBrush() noexcept {}
        // dispose
        void dispose() noexcept override;
    private:
        // begin point
        RefPtr<Asset::Bitmap>   m_spSource;
    }; 
}}



// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    /// <summary>
    /// Creates the specified property.
    /// </summary>
    /// <param name="c">The color.</param>
    /// <returns></returns>
    auto ColorBrush::Create(const ColorF& c) noexcept ->ColorBrush* {
        auto ptr = RubyGM::SmallAlloc(sizeof(ColorBrush));
        assert(ptr && "RubyGM::SmallAlloc cannot return nullptr");
        auto obj = new(ptr) Asset::ColorBrush(c);
        return obj;
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="ColorBrush"/> class.
    /// </summary>
    /// <param name="c">The color.</param>
    ColorBrush::ColorBrush(const ColorF& c) noexcept : m_color(c) {

    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void ColorBrush::dispose() noexcept {
        this->ColorBrush::~ColorBrush();
        RubyGM::SmallFree(this);
    }
    /// <summary>
    /// Gets the brush.
    /// </summary>
    /// <returns></returns>
    auto ColorBrush::GetBrush() noexcept ->IGMBrush* {
        // 没有笔刷
        if (!m_pBrush) {
            // 则创建
            IGMBrush* brush = nullptr;
            auto hr = Bridge::CreateBrushWithProp(m_color, &brush);
            // 检查错误
            if (FAILED(hr)) Game::SetLastErrorCode(hr);
            // 替换笔刷
            this->repleace_brush(std::move(brush));
        }
        // 返回已创建的笔刷
        return RubyGM::SafeAcquire(m_pBrush);
    }


    /// <summary>
    /// Creates the specified property.
    /// </summary>
    /// <param name="prop">The property.</param>
    /// <returns></returns>
    auto LinearGradientBrush::Create(
        const LinearBrush& prop) noexcept -> LinearGradientBrush*  {
        constexpr auto basic = sizeof(LinearGradientBrush);
        const auto extra = sizeof(GradientStop) * prop.count;
        assert(prop.count <= 256);
        if (const auto ptr = RubyGM::NormalAlloc(basic + extra)) {
            return new(ptr) LinearGradientBrush(prop);
        }
        return nullptr;
    }

    /// <summary>
    /// Linears the gradient brush.
    /// </summary>
    /// <param name="linear">The linear.</param>
    /// <returns></returns>
    LinearGradientBrush::LinearGradientBrush(
        const LinearBrush& linear) noexcept 
        : m_ptBegin(linear.begin), m_ptEnd(linear.end) {
        m_unused_brush = linear.count;
        std::memcpy(
            m_aStops, linear.stops,
            m_unused_brush * sizeof(m_aStops[0])
        );
    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void LinearGradientBrush::dispose() noexcept {
        this->LinearGradientBrush::~LinearGradientBrush();
        RubyGM::NormalFree(this);
    }
    /// <summary>
    /// Gets the brush.
    /// </summary>
    /// <returns></returns>
    auto LinearGradientBrush::GetBrush() noexcept ->IGMBrush* {
        // 没有笔刷
        if (!m_pBrush) {
            // 初始化
            IGMBrush* brush = nullptr; LinearBrush lb;
            lb.begin = m_ptBegin; lb.end = m_ptEnd;
            lb.count = m_unused_brush; lb.stops = m_aStops;
            // 则创建
            auto hr = Bridge::CreateBrushWithProp(lb, &brush);
            // 检查错误
            if (FAILED(hr)) Game::SetLastErrorCode(hr);
            // 替换笔刷
            this->repleace_brush(std::move(brush));
        }
        // 返回已创建的笔刷
        return RubyGM::SafeAcquire(m_pBrush);
    }


    /// <summary>
    /// Creates the specified property.
    /// </summary>
    /// <param name="prop">The property.</param>
    /// <returns></returns>
    auto RadialGradientBrush::Create(
        const RadialBrush& prop) noexcept -> RadialGradientBrush*  {
        constexpr auto basic = sizeof(RadialGradientBrush);
        const auto extra = sizeof(GradientStop) * prop.count;
        assert(prop.count <= 256);
        if (const auto ptr = RubyGM::NormalAlloc(basic + extra)) {
            return new(ptr) RadialGradientBrush(prop);
        }
        return nullptr;
    }

    /// <summary>
    /// Radials the gradient brush.
    /// </summary>
    /// <param name="linear">The linear.</param>
    /// <returns></returns>
    RadialGradientBrush::RadialGradientBrush(
        const RadialBrush& radial) noexcept 
        : m_ptCenter(radial.center), m_ptOffset(radial.offset),
        m_fRadiusX(radial.rx), m_fRadiusY(radial.ry) {
        m_unused_brush = radial.count;
        std::memcpy(
            m_aStops, radial.stops,
            m_unused_brush * sizeof(m_aStops[0])
        );
    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void RadialGradientBrush::dispose() noexcept {
        this->RadialGradientBrush::~RadialGradientBrush();
        RubyGM::NormalFree(this);
    }
    /// <summary>
    /// Gets the brush.
    /// </summary>
    /// <returns></returns>
    auto RadialGradientBrush::GetBrush() noexcept ->IGMBrush* {
        // 没有笔刷
        if (!m_pBrush) {
            // 初始化
            IGMBrush* brush = nullptr; RadialBrush rb;
            rb.center = m_ptCenter, rb.offset = m_ptOffset;
            rb.rx = m_fRadiusX; rb.ry = m_fRadiusY;
            rb.count = m_unused_brush; rb.stops = m_aStops;
            // 则创建
            auto hr = Bridge::CreateBrushWithProp(rb, &brush);
            // 检查错误
            if (FAILED(hr)) Game::SetLastErrorCode(hr);
            // 替换笔刷
            this->repleace_brush(std::move(brush));
        }
        // 返回已创建的笔刷
        return RubyGM::SafeAcquire(m_pBrush);
    }


    /// <summary>
    /// Creates the specified property.
    /// </summary>
    /// <param name="source">The source.</param>
    /// <returns></returns>
    auto BitmapBrush::Create(
        RefPtr<Asset::Bitmap>&& source) noexcept -> BitmapBrush*  {
        auto ptr = RubyGM::SmallAlloc(sizeof(BitmapBrush));
        assert(ptr && "SmallAlloc cannot return nullptr");
        return new(ptr) BitmapBrush(std::move(source));
    }

    /// <summary>
    /// Radials the gradient brush.
    /// </summary>
    /// <param name="source">The source.</param>
    /// <returns></returns>
    BitmapBrush::BitmapBrush(RefPtr<Asset::Bitmap>&& source) noexcept 
        :  m_spSource(std::move(source)) {
    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void BitmapBrush::dispose() noexcept {
        this->BitmapBrush::~BitmapBrush();
        RubyGM::SmallFree(this);
    }
    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    auto BitmapBrush::recreate() noexcept->Result {
        Super::recreate();
        return m_spSource->Recreate();
    }
    /// <summary>
    /// Gets the brush.
    /// </summary>
    /// <returns></returns>
    auto BitmapBrush::GetBrush() noexcept ->IGMBrush* {
        // 没有笔刷
        if (!m_pBrush) {
            // 获取位图
            if (const auto bitmap = m_spSource->GetBitmap()) {
                // 初始化
                IGMBrush* brush = nullptr; 
                // 则创建
                auto hr = Bridge::CreateBrushWithProp(bitmap, &brush);
                // 检查错误
                if (FAILED(hr)) Game::SetLastErrorCode(hr);
                // 替换笔刷
                this->repleace_brush(std::move(brush));
                // 释放引用
                bitmap->Release();
            }
        }
        // 返回已创建的笔刷
        return RubyGM::SafeAcquire(m_pBrush);
    }
}}

/// <summary>
/// Creates the color-brush asset.
/// </summary>
/// <param name="c">The color.</param>
/// <returns></returns>
auto RubyGM::Game::CreateBrushAsset(
    const ColorF& c) noexcept ->RefPtr<Asset::Brush> {
    auto brush = Asset::ColorBrush::Create(c);
    return std::move(RefPtr<Asset::Brush>(std::move(brush)));
}


/// <summary>
/// Creates the brush asset.
/// </summary>
/// <param name="lb">The lb.</param>
/// <returns></returns>
auto RubyGM::Game::CreateBrushAsset(
    const LinearBrush& lb) noexcept ->RefPtr<Asset::Brush> {
    // 试图创建
    if (auto brush = Asset::LinearGradientBrush::Create(lb)) {
        return std::move(RefPtr<Asset::Brush>(std::move(brush)));
    }
    // 内存不足则创建一般笔刷
    else {
        return std::move(Game::CreateBrushAsset(lb.stops->color));
    }
}

/// <summary>
/// Creates the brush asset.
/// </summary>
/// <param name="rb">The rb.</param>
/// <returns></returns>
auto RubyGM::Game::CreateBrushAsset(
    const RadialBrush& rb) noexcept ->RefPtr<Asset::Brush> {
    // 试图创建
    if (auto brush = Asset::RadialGradientBrush::Create(rb)) {
        return std::move(RefPtr<Asset::Brush>(std::move(brush)));
    }
    // 内存不足则创建一般笔刷
    else {
        return std::move(Game::CreateBrushAsset(rb.stops->color));
    }
}


/// <summary>
/// Creates the brush asset.
/// </summary>
/// <param name="bitmap">The bitmap.</param>
/// <returns></returns>
auto RubyGM::Game::CreateBrushAsset(
    const AssetBitmap& ab) noexcept -> RefPtr<Asset::Brush> {
    AssetBitmap bitmap(ab);
    auto brush = Asset::BitmapBrush::Create(std::move(bitmap));
    return std::move(RefPtr<Asset::Brush>(std::move(brush)));
}