#pragma once

#include <d2d1.h>

namespace simulation_app
{
    struct Vector3d
    {
        double x, y, z;

        Vector3d();

        Vector3d(double x, double y, double z);

        Vector3d operator+(const Vector3d& other) const;

        Vector3d operator-(const Vector3d& other) const;

        Vector3d operator*(double scalar) const;

        Vector3d operator/(double scalar) const;

        Vector3d operator+=(const Vector3d& other);

        double dot(const Vector3d& other) const;

        Vector3d cross(const Vector3d& other) const;

        float lengthSquared() const;

        void normalize();

        D2D1_POINT_2F toD2D() const;
        
    };
    
} // namespace simulation_app
