#include <core/rubygm.h>
#include <core/rgmmgr.h>
#include <core/graphics/rgmDrawable.h>
#include <memory>

// rubygm::drawable namespace
namespace RubyGM { namespace Drawable {
    // buffer for drawable head and tail
    char BUFFER_DRAWABLE_HEAD_TAIL[sizeof(Drawable::Base) * 2];
}}

/// <summary>
/// Initializes a new instance of the <see cref="CGMManager"/> class.
/// </summary>
RubyGM::CGMManager::CGMManager() noexcept {
    std::memset(
        Drawable::BUFFER_DRAWABLE_HEAD_TAIL, 
        0, 
        sizeof(Drawable::BUFFER_DRAWABLE_HEAD_TAIL)
    );
}

/// <summary>
/// Finalizes an instance of the <see cref="CGMManager"/> class.
/// </summary>
/// <returns></returns>
RubyGM::CGMManager::~CGMManager() noexcept {

}

/// <summary>
/// Initializes this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::CGMManager::Initialize() noexcept ->HRESULT {
    HRESULT hr = S_OK;
    // 创建链表头与尾巴
    if (SUCCEEDED(hr)) {
        const auto buffer = Drawable::BUFFER_DRAWABLE_HEAD_TAIL;
        m_pHead = reinterpret_cast<decltype(m_pHead)>(buffer);
        m_pTail = reinterpret_cast<decltype(m_pTail)>(buffer + sizeof(Drawable::Base));
        m_pHead->m_pNext = m_pTail;
        m_pTail->m_pPrve = m_pHead;
    }
    return hr;
}

/// <summary>
/// Uninitializes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::CGMManager::Uninitialize() noexcept {

}

/// <summary>
/// Uninitializes this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::GetLastDrawableObject() noexcept -> Drawable::Base* {
    return GMManager.GetDrawableTail();
}

// 单例初始化
RubyGM::CGMSingleton<RubyGM::CGMManager> RubyGM::CGMSingleton<RubyGM::CGMManager>::s_instance;