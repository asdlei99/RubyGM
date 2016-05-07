#pragma once
/**
* Copyright (c) 2015-2016 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Windows.h>
#include <cstdint>
#include "graphics/rgmGraphics.h"
#include "Util/rgmUtil.h"
#include "Util/rgmRuby.h"

// rubygm namespace
namespace RubyGM {
    // Drawable::Base
    namespace Drawable { class Base; }
    // rubygm manager - rubygm main singleton manager
    class CGMManager {
    public:
        // ctor
        CGMManager() noexcept;
        // dtor
        ~CGMManager() noexcept;
        // move ctor
        CGMManager(const CGMManager&) = delete;
        // copy dtor
        CGMManager(CGMManager&&) = delete;
    public:
        // initialize
        auto Initialize() noexcept ->HRESULT;
        // uninitialize
        void Uninitialize() noexcept;
        // get draw-able objects tail
        auto GetDrawableTail() const noexcept { return m_pTail; }
    private:
        // mruby state
        mrb_state*              m_pMRuby = nullptr;
        // head of drawable objects
        Drawable::Base*         m_pHead = nullptr;
        // tail of drawable objects
        Drawable::Base*         m_pTail = nullptr;
    public:
        // last error code
        uint32_t                error_code = 0;
    private:
        // unused
        uint32_t                m_unused = 0;
    public:
        //
        mrb_state*              mruby = nullptr;
    };
}

// RubyGM Manager
#define GMManager (*RubyGM::CGMSingleton<RubyGM::CGMManager>::s_instance.Ptr())
