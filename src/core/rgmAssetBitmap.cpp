#define _WIN32_WINNT 0x0A000001
#include <core/brush/rgmBrushStruct.h>
#include <core/asset/rgmAsset.h>
#include <core/util/rgmUtil.h>
#include <core/graphics/rgmGraphics.h>
#include <game/rgmGame.h>

// rubygm::asset namespace
namespace RubyGM { namespace Asset {
    // bitmap form stream asset
    class StreamBitmap final : public Bitmap {
    public:
        // create one instance
        static auto Create(const NBrushProperties& prop) noexcept ->StreamBitmap* ;
        // get bitmap
        auto GetBitmap() noexcept -> IGMBitmap* override;
        // recreate
        auto Recreate() noexcept->uint32_t override;
        // low occu
        void LowOccupancy() noexcept override;
    private:
        // ctor
        StreamBitmap() noexcept;
        // ctor
        ~StreamBitmap() noexcept;
        // dispose
        void dispose() noexcept override;
    private:
        // brush for this
        IGMBrush*               m_pBrush = nullptr;
        // brush properties
        NBrushProperties        m_propBrush;
    };
}}
