#pragma once

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
        
    };
    
} // namespace simulation_app
