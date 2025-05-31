#pragma once

#include "vector3d.h"
#include "particle.h"
#include <memory>
#include <functional>

namespace simulation_app
{
    class Barrier
    {
    private:
        Vector3d start_pos_;
        Vector3d end_pos_;
        std::function<void(std::shared_ptr<Particle>)> on_particle_fixed_func_;

    public:
        Barrier(const Vector3d& start_pos, const Vector3d& end_pos, std::function<void(std::shared_ptr<Particle>)> on_particle_fixed_func);

        bool contains(const Vector3d& pos) const;

        void render() const;

        void callOnParticleFixed(std::shared_ptr<Particle> particle) const;
    };
    
} // namespace simulation_app
