#include "barrier.h"
#include "d2d_resource.h"
#include "particle.h"
#include <memory>
#include <functional>

namespace simulation_app
{
    Barrier::Barrier(const Vector3d& start_pos, const Vector3d& end_pos, std::function<void(std::shared_ptr<Particle>)> barrier_func)
        : start_pos_(start_pos), end_pos_(end_pos), on_particle_fixed_func_(barrier_func)
    {
    }

    bool Barrier::contains(const Vector3d& pos) const
    {
        const Vector3d line = end_pos_ - start_pos_;
        const Vector3d pointVec = pos - start_pos_;
        
        const double t = pointVec.dot(line) / line.lengthSquared();
        if(t < 0 || t > 1)
        {
            return false;
        }
    
        const Vector3d proj = start_pos_ + line * t;
        return (pos - proj).lengthSquared() <= (0.04 * 0.04 / 4);
    }

    void Barrier::render() const
    {
        pRenderTarget->DrawLine(start_pos_.toD2D(), end_pos_.toD2D(), graphics::pBlackBrush, 0.04f);
    }

    void Barrier::callOnParticleFixed(std::shared_ptr<Particle> particle) const
    {
        on_particle_fixed_func_(particle);
    }

} // namespace simulation_app