#include "barrier.h"
#include "d2d_resource.h"

namespace simulation_app
{
    Barrier::Barrier(const Vector3d& start_pos, const Vector3d& end_pos)
        : start_pos_(start_pos), end_pos_(end_pos)
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
        return (pos - proj).lengthSquared() <= (0.2 * 0.2 / 4);
    }

    void Barrier::render() const
    {
        pRenderTarget->DrawLine(start_pos_.toD2D(), end_pos_.toD2D(), graphics::pBarrierBrush, 0.2f);
    }

} // namespace simulation_app