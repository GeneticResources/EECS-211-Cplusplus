#pragma once

#include "color.h"

namespace graphics
{

class color_blender
{
public:
    color_blender() noexcept;

    color_blender& add(double weight, const Color&) noexcept;
    Color get() const noexcept;

private:
    double red_    = 0;
    double green_  = 0;
    double blue_   = 0;
    double alpha_  = 0;
    double weight_ = 0;
};

color_blender& operator<<(color_blender&, const Color&) noexcept;

} // end namespace graphics
