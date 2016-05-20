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

#include "../rubygm.h"

#ifndef RUBYGM_NOVTABLE
#ifdef _MSC_VER
#define RUBYGM_NOVTABLE __declspec(novtable)
#else
#define RUBYGM_NOVTABLE
#endif
#endif

// rubygm namespace
namespace RubyGM {
    // bridge::uigame
    namespace Bridge { class UIGame; }
    // base namepsace
    namespace Base {
        // resource class
        class RUBYGM_NOVTABLE Resource {
            // friend
            friend class Bridge::UIGame;
        public:
            // add ref-count
            auto AddRef() noexcept->uint32_t;
            // release ref-count
            auto Release() noexcept ->uint32_t;
            // ctor
            Resource() noexcept;
            // dtor
            ~Resource() noexcept;
        private:
            // dispose object
            virtual void dispose() noexcept = 0;
        protected:
            // recreate resource
            virtual auto recreate() noexcept -> Result = 0;
        public:
            // after recreate:
            void AfterRecreate() noexcept { m_bCouldRecreate = true; }
            // recreate:
            auto Recreate() noexcept->Result;
        protected:
            // ref-count
            uint32_t            m_cRef = 1;
            // can recreat
            bool                m_bCouldRecreate = true;
            // -- nameless
            bool                m_bUnused = false;
            // custom data
            uint16_t            m_u16Data = 0;
        public:
            // prve
            Resource*           m_pPrve;
            // next
            Resource*           m_pNext;
        };
    }
}

