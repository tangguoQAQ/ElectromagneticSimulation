#pragma once

#include "vector3d.h"
#include "field.h"

namespace simulation_app
{
    struct FieldArgs;

    class Particle {
    private:
        Vector3d pos_;  // m
        Vector3d v;  // m/s
        double m;        // kg
        double q;      // C

    public:
        Particle(const Vector3d& position, const Vector3d& velocity, double mass, double charge);

        inline const Vector3d& getPos() const
        {
            return pos_;
        }

        inline const Vector3d& getV() const
        {
            return v;
        }

        inline double getM() const
        {
            return m;
        }

        inline double getQ() const
        {
            return q;
        }

        void update(double dt, const FieldArgs& fieldArgs);

        void render() const;
    };

} // namespace simulation_app

