#include "particle.h"
#include "vector3d.h"
#include "d2d_resource.h"

namespace simulation_app
{
    Particle::Particle(const Vector3d& position, const Vector3d& velocity, double mass, double charge)
        : pos_(position), v(velocity), m(mass), q(charge)
    {
    }

    void Particle::update(double dt, const FieldArgs& fieldArgs)
    {
        const Vector3d& E = fieldArgs.E,
                B = fieldArgs.B;

        v += ((E * q) / m) * (dt/2.0);

        const Vector3d t = (B * q) / 2*m * dt;
        const Vector3d s = t*2 / (1 + t.dot(t));

        const Vector3d v_minus = v; 
        const Vector3d v_prime = v_minus + v_minus.cross(t); 
        v = v_minus + v_prime.cross(s); 

        v += ((E * q) / m) * (dt/2.0);

        pos_ += v * dt;
    }

    void Particle::render() const
    {
        pRenderTarget->FillEllipse(D2D1::Ellipse(
            D2D1::Point2F(pos_.x, pos_.y),
            0.2, 0.2),
            graphics::pParticleBrush);
    }

    
} // namespace simulation_app

/*
数值案例（电子在B=0.1T磁场中）：

参数	值
初速度	(1e6, 0, 0) m/s
Δt	1e-9 s
t向量	(0, 0, 4.4e-5)
终速度	(0.9999e6, 4.4e5, 0) → 保持模长
*/