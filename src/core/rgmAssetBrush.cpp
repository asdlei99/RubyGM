#define _WIN32_WINNT 0x0A000001
#include <core/brush/rgmBrushStruct.h>
#include <core/asset/rgmAsset.h>
#include <core/util/rgmUtil.h>
#include <core/graphics/rgmGraphics.h>
#include <bridge/rgmluiBridge.h>
#include <game/rgmGame.h>

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    // normal brush asset
    class NormalBrush final : public Brush {
    public:
        // create one instance
        static auto Create(const NBrushProperties& prop) noexcept ->NormalBrush* ;
        // get brush
        auto GetBrush() noexcept -> IGMBrush* override;
        // low occu
        void LowOccupancy() noexcept override;
    protected:
        // recreate
        auto recreate() noexcept->Result override;
    private:
        // ctor
        NormalBrush(const NBrushProperties&) noexcept;
        // ctor
        ~NormalBrush() noexcept;
        // dispose
        void dispose() noexcept override;
    private:
        // brush for this
        IGMBrush*               m_pBrush = nullptr;
        // brush properties
        NBrushProperties        m_propBrush;
    };
    /// <summary>
    /// Creates the specified property.
    /// </summary>
    /// <param name="prop">The property.</param>
    /// <returns></returns>
    auto NormalBrush::Create(const NBrushProperties& prop) noexcept ->NormalBrush* {
        auto ptr = RubyGM::SmallAlloc(sizeof(NormalBrush));
        assert(ptr && "RubyGM::SmallAlloc cannot return nullptr");
        auto obj = new(ptr) Asset::NormalBrush(prop);
        return obj;
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="NormalBrush"/> class.
    /// </summary>
    /// <param name="prop">The property.</param>
    NormalBrush::NormalBrush(const NBrushProperties& prop)
        noexcept : m_propBrush(prop) {

    }
    /// <summary>
    /// Finalizes an instance of the <see cref="NormalBrush"/> class.
    /// </summary>
    /// <returns></returns>
    NormalBrush::~NormalBrush() noexcept {
        RubyGM::SafeRelease(m_pBrush);
    }
    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    auto NormalBrush::recreate() noexcept -> Result {
        // 释放资源, 等待下一次重建
        RubyGM::SafeRelease(m_pBrush);
        return Result(S_OK);
    }
    /// <summary>
    /// Lows the occupancy.
    /// </summary>
    /// <returns></returns>
    void NormalBrush::LowOccupancy() noexcept {
        this->LowOccupancyHelper(m_pBrush);
    }
    /// <summary>
    /// Disposes this instance.
    /// </summary>
    /// <returns></returns>
    void NormalBrush::dispose() noexcept {
        this->NormalBrush::~NormalBrush();
        RubyGM::SmallFree(this);
    }
    /// <summary>
    /// Gets the brush.
    /// </summary>
    /// <returns></returns>
    auto NormalBrush::GetBrush() noexcept ->IGMBrush* {
        // 没有笔刷则创建
        if (!m_pBrush) {
            auto hr = Bridge::CreateBrushWithProp(m_propBrush, &m_pBrush);
            if (!m_pBrush) Game::SetLastErrorCode(hr);
        }
        // 返回已创建的笔刷
        return RubyGM::SafeAcquire(m_pBrush);
    }
}}

/// <summary>
/// Creates the brush asset.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
auto RubyGM::Game::CreateBrushAsset(
    const NBrushProperties& bnp) noexcept -> Asset::Brush& {
    return *Asset::NormalBrush::Create(bnp);
}
