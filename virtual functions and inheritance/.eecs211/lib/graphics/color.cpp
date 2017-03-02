#include "color.h"

namespace graphics
{

using graphics::color32;

Color::Color() noexcept : Color{0, 0, 0, 0}
{ }

Color::Color(sample red, sample green, sample blue, sample alpha) noexcept
        : red_{red}, green_{green}, blue_{blue}, alpha_{alpha}
{
    if (alpha == 0) red_ = green_ = blue_ = 0;
}

static sample to_sample(color32::byte b) noexcept
{
    return sample{b / static_cast<double>(color32::BYTE_MAX)};
}

Color::Color(color32 c) noexcept
        : Color{to_sample(c.red()),
                to_sample(c.green()),
                to_sample(c.blue()),
                to_sample(c.alpha())}
{ }

static color32::byte to_byte(sample s) noexcept
{
    return static_cast<color32::byte>(color32::BYTE_MAX * s.value());
}

Color::operator graphics::color32() const noexcept
{
    return color32{to_byte(red()),
                   to_byte(green()),
                   to_byte(blue()),
                   to_byte(alpha())};
}

Color const Color::transparent{0, 0, 0, 0};
Color const Color::white{1, 1, 1, 1};
Color const Color::black{0, 0, 0, 1};

bool opaque(const Color& c) noexcept
{
    return c.alpha() == 1;
}

bool transparent(const Color& c) noexcept
{
    return c.alpha() == 0;
}

Color overlay(const Color& fg, const Color& bg) noexcept
{
    if (opaque(fg) || transparent(bg)) return fg;
    if (transparent(fg)) return bg;

    return interpolate(bg, fg.alpha(), Color{fg.red(), fg.green(), fg.blue()});
}

Color interpolate(const Color& a, sample weight, const Color& b)
noexcept
{
    auto pre_red =
                 interpolate(a.alpha() * a.red(), weight, b.alpha() * b.red());
    auto pre_green =
                 interpolate(a.alpha() * a.green(), weight, b.alpha() * b.green());
    auto pre_blue =
                 interpolate(a.alpha() * a.blue(), weight, b.alpha() * b.blue());
    auto new_alpha = interpolate(a.alpha(), weight, b.alpha());

    return {pre_red / new_alpha, pre_green / new_alpha, pre_blue / new_alpha,
            new_alpha};
}

Partial_blend::Partial_blend(Color left, sample weight)
        : left_{left}, weight_{weight}
{ }

Partial_blend operator<(const Color& left, sample factor)
{
    return Partial_blend{left, factor};
}

Color operator>(const Partial_blend& pb, const Color& right)
{
    return interpolate(pb.left_, pb.weight_, right);
}

} // end namespace graphics
