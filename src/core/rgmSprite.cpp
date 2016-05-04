// M$
#pragma warning(disable: 4290)
#pragma warning(disable: 4200)
#include <core/rubygm.h>
#include <core/graphics/rgmSprite.h>

/// <summary>
/// Initializes a new instance of the <see cref="CGMSprite"/> class.
/// </summary>
RubyGM::CGMSprite::CGMSprite() noexcept {

}

/// <summary>
/// Finalizes an instance of the <see cref="CGMSprite"/> class.
/// </summary>
/// <returns></returns>
RubyGM::CGMSprite::~CGMSprite() noexcept {

}

#include <core/graphics/drawable/rgmLine.h>
using namespace RubyGM;

int test() noexcept {
    RubyGM::CGMSprite sprite;
    CGMPtr<Drawable::Line> b = RubyGM::Drawable::Line::CreateSP();
    sprite.SetDrawable(b);

    return 0;
}