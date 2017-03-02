#pragma once

#include "sample.h"
#include "raster/color32.h"

#include <cstddef>

namespace graphics
{

class Color
{
public:
    Color() noexcept; // transparent
    Color(sample red, sample green, sample blue, sample alpha = 1) noexcept;

    Color(graphics::color32) noexcept;
    operator graphics::color32() const noexcept;

    static Color const transparent;
    static Color const white;
    static Color const black;

    sample red()   const noexcept { return red_;   }
    sample green() const noexcept { return green_; }
    sample blue()  const noexcept { return blue_;  }
    sample alpha() const noexcept { return alpha_; }

private:
    sample red_, green_, blue_, alpha_;
};

Color grayscale(const Color&) noexcept;
Color overlay(const Color& foreground, const Color& background) noexcept;
Color interpolate(const Color& a, sample weight, const Color& b) noexcept;

class Partial_blend
{
    Partial_blend(Color left, sample weight);

    Color  left_;
    sample weight_;

    friend Partial_blend operator<(const Color&, sample);
    friend Color operator>(const Partial_blend&, const Color&);
};

Partial_blend operator<(const Color&, sample);
Color operator>(const Partial_blend&, const Color&);

} // namespace graphics
