#include <d2d1.h>

#include "rect.h"

namespace simulation_app
{
    Rect::Rect() : left(0), top(0), right(0), bottom(0)
    {
    }

    Rect::Rect(double left, double top, double right, double bottom)
        : left(left), top(top), right(right), bottom(bottom)
    {
    }

    Rect::Rect(const Rect& other) = default;

    bool Rect::contains(double x, double y) const
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    D2D1_RECT_F Rect::toD2D() const
    {
        return D2D1::RectF(left, top, right, bottom);
    }

    Rect Rect::make(double x, double y, double width, double height)
    {
        return Rect(x, y, x + width, y + height);
    }

} // namespace simulation_app