#include <core/rubygm.h>
#include <core/graphics/rgmGraphics.h>
#include <core/Drawable/rgmLine.h>
#include <core/util/rgmImpl.h>
//#include <game/rgmGame.h>


/// <summary>
/// Initializes a new instance of the <see cref="Line"/> class.
/// </summary>
/// <param name="mode">The mode.</param>
RubyGM::Drawable::Line::Line(const LineStatus& ls) noexcept :
Super(ls),
point0(ls.point0),
point1(ls.point1),
stroke_width(ls.stroke_width) {

}


/// <summary>
/// Finalizes an instance of the <see cref="Line"/> class.
/// </summary>
/// <returns></returns>
RubyGM::Drawable::Line::~Line() noexcept {

}

/// <summary>
/// Creates this instance.
/// </summary>
/// <returns></returns>
auto RubyGM::Drawable::Line::Create(const LineStatus& ls) noexcept -> Line* {
    return new(std::nothrow) Line(ls);
}

/// <summary>
/// Disposes this instance.
/// </summary>
/// <returns></returns>
void RubyGM::Drawable::Line::dispose() noexcept {
    delete this;
}

/// <summary>
/// Renders the specified rc.
/// </summary>
/// <param name="rc">The rc.</param>
/// <returns></returns>
void RubyGM::Drawable::Line::Render(IGMRednerContext& rc) const noexcept {
    assert(m_pBrush && "bad brush");
    this->before_render();
    rc.DrawLine(
        impl::d2d(this->point0), 
        impl::d2d(this->point1), 
        m_pBrush, 
        this->stroke_width,
        nullptr
    );
}

/// <summary>
/// Recreates this instance.
/// </summary>
/// <returns></returns>
/*auto RubyGM::Drawable::Line::Recreate() noexcept -> uint32_t {
    return uint32_t(S_OK);
}*/

