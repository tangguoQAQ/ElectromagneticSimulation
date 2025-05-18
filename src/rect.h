#pragma once

#include <d2d1.h>

namespace simulation_app
{
    struct Rect
    {
        double left, top, right, bottom;

        Rect();
        Rect(double left, double top, double right, double bottom);
        
        Rect(const Rect& other);

        bool contains(double x, double y) const;

        D2D1_RECT_F toD2D() const;

        static Rect make(double x, double y, double width, double height);
    };
    
} // namespace simulation_app
