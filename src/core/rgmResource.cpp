#include <core/resource/rgmFontResource.h>
#include <core/graphics/rgmGraphics.h>
#include <core/text/rgmTextDetail.h>
#include <core/util/rgmUtil.h>
#include <cwchar>
#include <memory>

// rubygm::resource namespace
namespace RubyGM { namespace Resource {
    /// <summary>
    /// Initializes a new instance of the <see cref="Font"/> class.
    /// </summary>
    /// <param name="prop">The property.</param>
    Font::Font(const FontProperties& prop, size_t len) noexcept {
        std::memcpy(m_bufFontProp, &prop, len);
    }

    /// <summary>
    /// Finalizes an instance of the <see cref="Font"/> class.
    /// </summary>
    /// <returns></returns>
    Font::~Font() noexcept {
        RubyGM::SafeRelease(m_pTextFormat);
    }

    /// <summary>
    /// Recreates this instance.
    /// </summary>
    /// <returns></returns>
    auto Font::Recreate() noexcept -> uint32_t {
        // 文本格式是CPU资源, 没有有必要重建
        return uint32_t(S_OK);
    }
    /// <summary>
    /// Lows the occupancy.
    /// </summary>
    /// <returns></returns>
    void Font::LowOccupancy() noexcept {
        // 在没有被其他对象引用时候释放
        Resource::LowOccupancyHelper(m_pTextFormat);
    }

    /// <summary>
    /// Create a <see cref="Font"/> object with the specified prop . never 
    /// </summary>
    /// <param name="">The .</param>
    /// <returns></returns>
    auto Font::Create(const FontProperties& prop) noexcept -> Font& {
        auto len = std::wcslen(prop.name);
        size_t size = sizeof(Font) + (len + 1) * sizeof(wchar_t);
        auto ptr = RubyGM::SmallAlloc(size);
        assert(ptr && "RubyGM::SmallAlloc cannot return nullptr");
        auto obj = new(ptr) Resource::Font(prop, size);
        return *obj;
    }

    /// <summary>
    /// Releases unmanaged and - optionally - managed resources.
    /// </summary>
    /// <returns></returns>
    void Font::Dispose() noexcept {
        // 调用析构函数
        this->Font::~Font();
        // 释放数据
        RubyGM::SmallFree(this);
    }
    /// <summary>
    /// Gets the font.
    /// </summary>
    /// <returns></returns>
    auto Font::GetFont() noexcept ->IGMFont* {
        // 拥有直接返回
        if (m_pTextFormat) {
            return RubyGM::SafeAcquire(m_pTextFormat);
        }
        // 没有则创建
        else {
            return nullptr;
        }
    }
}}



