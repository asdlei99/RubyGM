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


#include <cstdint>
#include "rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include "../Util/rgmStruct.h"
#include <list>


// rubygm namespace
namespace RubyGM { 
    // list for data
    template<typename T> using List = std::list<T>;
    // sprte status
    struct SprteStatus {
        // position
        float       x, y;
        // scale
        float       zoomx, zoomy;
        // origin position
        float       ox, oy;
        // rotation
        float       rotation;
    }; 
    // default status
    const extern SprteStatus DEFAULT_STATUS;
    // sprite, game graphics element
    class CGMSprite {
        // bit list
        enum BitIndex : size_t {
            // world changed
            Index_WorldChanged = 0,
            // visible
            Index_Visible,
        };
        // frend class for list
        friend List<CGMSprite>;
    public:
        // clip zone
        static constexpr float CLIP_ZONE = 60'000.f;
        // antialias-mode
        enum AntialiasMode : uint8_t {
            // The edges of each primitive are antialiased sequentially.
            Mode_PerPrimitive = 0,
            // Each pixel is rendered if its pixel center is contained by the geometry.
            Mode_Aliased = 1,
        };
        // set drawable object
        template<typename T> void SetDrawable(const T& obj) noexcept {
            m_spDrawable = obj;
        }
        // set drawable object
        template<typename T> void SetDrawable(T&& obj) noexcept {
            m_spDrawable = std::move(obj);
        }
    public:
        // add a new sprite to list
        static auto AddNew(const SprteStatus& = DEFAULT_STATUS) noexcept ->CGMSprite*;
        // ctor
        CGMSprite(CGMSprite* parent) noexcept;
        // dtor
        ~CGMSprite() noexcept;
        // copy ctor
        CGMSprite(const CGMSprite&) noexcept = delete;
        // move ctor
        CGMSprite(CGMSprite&&) noexcept = delete;
        // render
        void Render(IGMRednerContext& ) noexcept;
        // root render
        void RootRender(IGMRednerContext& ) const noexcept;
        // add child
        auto AddChild() /*throw(std::exception)*/ ->CGMSprite&;
        // get
        void Get(SprteStatus&) const noexcept;
        // set
        void Set(const SprteStatus&) noexcept;
        // set visible
        void SetVisible(bool v) noexcept { this->set_visible(v); }
        // get visible
        bool GetVisible() const noexcept { return this->is_visible(); }
        // set transform directly, the sprite status will be invalid
        void SetTransform(const Matrix3X2F&) noexcept;
    public:
        // get x
        auto GetX() const noexcept { return m_status.x; }
        // get y
        auto GetY() const noexcept { return m_status.y; }
        // get origin x
        auto GetOX() const noexcept { return m_status.ox; }
        // get origin y
        auto GetOY() const noexcept { return m_status.oy; }
        // get x-scale
        auto GetZoomX() const noexcept { return m_status.zoomy; }
        // get y-scale
        auto GetZoomY() const noexcept { return m_status.zoomy; }
        // get rotation
        auto GetRotation() const noexcept { return m_status.rotation; }
    public:
        // get x
        void SetX(float x) noexcept { m_status.x = x; this->set_world_changed(); }
        // get y
        void SetY(float y) noexcept { m_status.y = y; this->set_world_changed(); }
        // get origin x
        void SetOX(float x) noexcept { m_status.ox = x; this->set_world_changed(); }
        // get origin y
        void SetOY(float y) noexcept { m_status.oy = y; this->set_world_changed(); }
        // get origin x
        void SetZoomX(float x) noexcept { m_status.zoomx = x; this->set_world_changed(); }
        // get origin y
        void SetZoomY(float y) noexcept { m_status.zoomy = y; this->set_world_changed(); }
        // get rotation
        void SetRotation(float r) noexcept { m_status.rotation = r; this->set_world_changed(); }
    public:
        // get clip rect
        const auto& GetClipRect() const noexcept { return m_rcClip; }
        // set clip rect
        void SetClipRect(const RectF& rc) noexcept { m_rcClip = rc; }
        // clear clip rect
        void ClearClipRect() noexcept { m_rcClip = { -CLIP_ZONE, -CLIP_ZONE, CLIP_ZONE, CLIP_ZONE }; }
     protected:
        // make the world transform
        void make_transform(/*OUT*/Matrix3X2F& transform) const noexcept;
        // clear world changed
        void clear_world_changed() noexcept { m_baList.SetFalse(Index_WorldChanged); }
        // set world changed
        void set_world_changed() noexcept { m_baList.SetTrue(Index_WorldChanged); }
        // is world changed
        bool is_world_changed() const { return m_baList.Test(Index_WorldChanged); }
        // is visible
        void set_visible(bool v) noexcept { m_baList.SetTo(Index_Visible, v); }
        // is visible
        void set_visible() noexcept { m_baList.SetTrue(Index_Visible); }
        // is visible
        bool is_visible() const { return m_baList.Test(Index_Visible); }
    protected:
        // drawable object
        CGMPtr<Drawable::Base>  m_spDrawable = nullptr;
        // parent
        CGMSprite*              m_pParent = nullptr;
        // children
        List<CGMSprite>         m_ltChildren;
        // world transform
        Matrix3X2F              m_matWorld;
        // clip rect
        RectF                   m_rcClip;
        // sprte status
        SprteStatus             m_status;
        // bool array
        CGMBitArray<uint16_t>   m_baList;
        // unused
        uint8_t                 unused = 0;
    public:
        // antialias-mode
        AntialiasMode           antialias_mode = Mode_PerPrimitive;
    };
}