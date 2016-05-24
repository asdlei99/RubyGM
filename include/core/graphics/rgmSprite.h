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
#include "../drawable/rgmDrawable.h"
#include "../Util/rgmUtil.h"
#include "../Util/rgmStruct.h"
#include <list>
#include "../../test/rgmTest.h"

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
        // skew angle
        float       skewx, skewy;
        // rotation angle
        float       rotation;
        // z postion
        int32_t     z;
    }; 
    // default status
    const extern SprteStatus DEFAULT_STATUS;
    // sprite, game graphics element
    class CGMSprite : public CGMTestModule {
        // bit list
        enum BitIndex : size_t {
            // world changed
            Index_WorldChanged = 0,
            // child z changed
            Index_ChildZ,
            // visible
            Index_Visible,
            // strict clip
            Index_StrictClip,
        };
        // frend class for list
        friend List<CGMSprite>;
    public:
        // clip zone
        static constexpr float CLIP_ZONE = 600'000.f;
        // antialias-mode
        enum AntialiasMode : uint8_t {
            // same as prev sprite
            Mode_None = 255ui8,
            // The edges of each primitive are antialiased sequentially.
            Mode_PerPrimitive = 0,
            // Each pixel is rendered if its pixel center is contained by the geometry.
            Mode_Aliased = 1,
        };
        // set drawable object
        void SetDrawable(CGMPtrA<Drawable::Object>&& obj) noexcept;
        // set drawable object
        void SetDrawable(const CGMPtrA<Drawable::Object>& obj) noexcept;
        // get drawable, will add ref-count
        auto GetDrawable() const noexcept { return m_spDrawable; }
        // ref drawable, won't add ref-count
        auto&RefDrawable() const noexcept { return m_spDrawable; }
    public:
        // ctor
        CGMSprite(const SprteStatus& ss, CGMSprite* parent) noexcept;
        // dtor
        ~CGMSprite() noexcept;
        // copy ctor
        CGMSprite(const CGMSprite&) noexcept = delete;
        // move ctor
        CGMSprite(CGMSprite&&) noexcept = delete;
        // clear sprite
        void Clear() noexcept;
        // render
        void Render(IGMRenderContext& ) noexcept;
        // root render
        void RootRender(IGMRenderContext& ) const noexcept;
        // add child
        auto AddChild(const SprteStatus& ss) noexcept ->CGMSprite*;
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
        // get world transform
        auto&GetTransform() const noexcept { return m_matWorld; }
        // set clip type: strict?
        void SetStrictClip(bool sc) noexcept { this->set_strict_clip(sc); }
        // compute normal scale factor, without skew
        auto ComputeScaleFactorEz1() const noexcept ->float;
        // compute normal scale factor, without skew
        auto ComputeScaleFactorEz2() const noexcept ->SizeF;
        // compute complete scale factor, with skew
        auto ComputeScaleFactorEx1() const noexcept ->float;
        // compute complete scale factor, with skew
        auto ComputeScaleFactorEx2() const noexcept ->SizeF;
    public:
        // get x
        auto GetX() const noexcept { return m_status.x; }
        // get y
        auto GetY() const noexcept { return m_status.y; }
        // get z
        auto GetZ() const noexcept { return m_status.z; }
        // get origin x
        auto GetOX() const noexcept { return m_status.ox; }
        // get origin y
        auto GetOY() const noexcept { return m_status.oy; }
        // get skew angle x
        auto GetSkewX() const noexcept { return m_status.skewx; }
        // get skew angle y
        auto GetSkewY() const noexcept { return m_status.skewy; }
        // get x-scale
        auto GetZoomX() const noexcept { return m_status.zoomy; }
        // get y-scale
        auto GetZoomY() const noexcept { return m_status.zoomy; }
        // get rotation
        auto GetRotation() const noexcept { return m_status.rotation; }
    public:
        // set x
        void SetX(float x) noexcept { m_status.x = x; this->set_world_changed(); }
        // set y
        void SetY(float y) noexcept { m_status.y = y; this->set_world_changed(); }
        // set z
        void SetZ(int32_t z) noexcept { m_status.z = z; m_pParent->set_childz_changed(); }
        // set origin x
        void SetOX(float x) noexcept { m_status.ox = x; this->set_world_changed(); }
        // set origin y
        void SetOY(float y) noexcept { m_status.oy = y; this->set_world_changed(); }
        // set skew x
        void SetSkewX(float x) noexcept { m_status.skewx = x; this->set_world_changed(); }
        // set skew y
        void SetSkewY(float y) noexcept { m_status.skewy = y; this->set_world_changed(); }
        // set origin x
        void SetZoomX(float x) noexcept { m_status.zoomx = x; this->set_world_changed(); }
        // set origin y
        void SetZoomY(float y) noexcept { m_status.zoomy = y; this->set_world_changed(); }
        // set rotation
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
        void clear_world_changed() noexcept { m_baList.SetFalse<Index_WorldChanged>(); }
        // set world changed
        void set_world_changed() noexcept { m_baList.SetTrue<Index_WorldChanged>(); }
        // is world changed
        bool is_world_changed() const { return m_baList.Test<Index_WorldChanged>(); }
        // is visible
        void set_visible(bool v) noexcept { m_baList.SetTo<Index_Visible>(v); }
        // is visible
        void set_visible() noexcept { m_baList.SetTrue<Index_Visible>(); }
        // is visible
        bool is_visible() const { return m_baList.Test<Index_Visible>(); }
        // is visible
        bool is_childz_changed() const { return m_baList.Test<Index_ChildZ>(); }
        // set child z changed
        void set_childz_changed(bool v) { m_baList.SetTo<Index_ChildZ>(v); }
        // set child z changed
        void set_childz_changed() { m_baList.SetTrue<Index_ChildZ>(); }
        // clear child z changed
        void clear_childz_changed() { m_baList.SetFalse<Index_ChildZ>(); }
        // clear strict clip
        void clear_strict_clip() { m_baList.SetFalse<Index_StrictClip>(); }
        // set strict clip
        void set_strict_clip() { m_baList.SetTrue<Index_StrictClip>(); }
        // set strict clip
        void set_strict_clip(bool b) { m_baList.SetTo<Index_StrictClip>(b); }
        // set strict clip
        bool is_strict_clip() const { return m_baList.Test<Index_StrictClip>(); }
    protected:
        // sort children
        void sort_children() noexcept;
        // push clip
        void push_clip(IGMRenderContext&)  noexcept;
        // pop clip
        void pop_clip(IGMRenderContext& rc)  noexcept;
        // update
        void update() noexcept;
    protected:
        // drawable object
        CGMPtrA<Drawable::Object>   m_spDrawable = nullptr;
        // parent
        CGMSprite*                  m_pParent = nullptr;
        // children
        List<CGMSprite>             m_ltChildren;
        // world transform
        Matrix3X2F                  m_matWorld;
        // clip rect
        RectF                       m_rcClip;
        // sprte status
        SprteStatus                 m_status;
        // unused
        uint32_t                    m_unused_u32 = 233;
        // bool array
        CGMBitArray<uint16_t>       m_baList;
        // unused
        uint8_t                     m_unused_u8 = 233;
    public:
        // antialias-mode
        AntialiasMode               antialias_mode = Mode_None;
    };
}