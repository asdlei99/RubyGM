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


#include <cstdint>
#include "rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include "../Util/rgmStruct.h"

// rubygm namespace
namespace RubyGM { 
    // sprite, game graphics element
    class CGMSprite {
    public:
        // set drawable object
        template<typename T> void SetDrawable(const T& obj) noexcept {
            m_spDrawable = obj;
        }
        // set drawable object
        template<typename T> void SetDrawable(T&& obj) noexcept {
            m_spDrawable = std::move(obj);
        }
        // ctor
        CGMSprite() noexcept;
        // dtor
        ~CGMSprite() noexcept;
    protected:
        // copy ctor
        CGMSprite(const CGMSprite&) =delete;
        // move ctor
        CGMSprite(CGMSprite&&) =delete;
    protected:
        // drawable object
        CGMPtr<Drawable::Base>  m_spDrawable = nullptr;
        // world transform
        Matrix3X2F              m_matWorld;
    };
}