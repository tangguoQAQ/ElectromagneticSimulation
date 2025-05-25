#pragma once

#include "vector3d.h"

namespace simulation_app
{
    class Barrier
    {
    private:
        Vector3d start_pos_;
        Vector3d end_pos_;

    public:
        Barrier(const Vector3d& start_pos, const Vector3d& end_pos);

        bool contains(const Vector3d& pos) const;

        void render() const;
    };
    
} // namespace simulation_app
