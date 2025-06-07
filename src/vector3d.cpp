#include <cmath>

#include <d2d1.h>

#include "vector3d.h"

namespace simulation_app
{
    Vector3d::Vector3d() : x(0), y(0), z(0)
    {
    }

    Vector3d::Vector3d(double x, double y, double z) : x(x), y(y), z(z)
    {
    }

    Vector3d Vector3d::operator+(const Vector3d& other) const
    {
        return Vector3d(x + other.x, y + other.y, z + other.z);
    }

    Vector3d Vector3d::operator-(const Vector3d& other) const
    {
        return Vector3d(x - other.x, y - other.y, z - other.z);
    }

    Vector3d Vector3d::operator*(double scalar) const
    {
        return Vector3d(x * scalar, y * scalar, z * scalar);
    }

    Vector3d Vector3d::operator/(double scalar) const
    {
        return Vector3d(x / scalar, y / scalar, z / scalar);
    }

    Vector3d Vector3d::operator+=(const Vector3d& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    double Vector3d::dot(const Vector3d& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3d Vector3d::cross(const Vector3d& other) const
    {
        return Vector3d(y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x);
    }

    float Vector3d::lengthSquared() const
    {
        return x * x + y * y;
    }

    Vector3d Vector3d::normalize() const { 
        float len = std::sqrt(lengthSquared());
        return Vector3d(x / len, y / len, z / len);
    }

    D2D1_POINT_2F Vector3d::toD2D() const
    {
        return D2D1::Point2F(x, y);
    }
    
} // namespace simulation_app
