#pragma once
/**
* Copyright (c) 2015-2015 dustpg   mailto:dustpg@gmail.com
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

// rubygm namespace
namespace RubyGM { 
    // rubygm manager - rubygm main singleton manager
    class CGMManager {
    public:
        // ctor
        CGMManager() noexcept {}
        // dtor
        ~CGMManager() noexcept {}
        // move ctor
        CGMManager(const CGMManager&) = delete;
        // copy dtor
        CGMManager(CGMManager&&) = delete;
    public:
        // initialize
        auto Initialize() noexcept->HRESULT;
        // uninitialize
        void Uninitialize() noexcept;
    private:
        // rubygm graphics
        CGMGraphics             m_oGraphics;
        // mruby state
        mrb_state*              m_pMRuby = nullptr;
    public:
        //
        mrb_state*              mruby = nullptr;
    };
    // singleton
    template<typename T>
    struct CGMSingleton {
        // ptr
        auto Ptr() noexcept { return reinterpret_cast<T*>(this->buffer); }
        // create
        template<typename ...Args> auto Create(Args... args) {
#ifdef _DEBUG
            assert(state == 0 && "create this after destory!");
            state++;
#endif
            return new(this->buffer) T(args...);
        }
        // destory
        auto Destory() noexcept {
#ifdef _DEBUG
            assert(state != 0 && "destory this after create!");
            --state;
#endif
            this->Ptr()->~T();
        }
        // buffer for  singleton
        alignas(T)  char    buffer[sizeof(T)];
#ifdef _DEBUG
        // state
        size_t              state = 0;
#endif
        // singleton
        static CGMSingleton<T>      s_instance;
    };
#define GMManager (*RubyGM::CGMSingleton<RubyGM::CGMManager>::s_instance.Ptr())
}