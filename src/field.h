#pragma once

#include "particle.h"
#include <memory>
#include <vector>
#include "rect.h"
#include "vector3d.h"

namespace simulation_app
{
    class Particle;

    using ParticleVector = std::vector<std::shared_ptr<Particle>>;

    struct FieldArgs
    {
        Vector3d E;
        Vector3d B;
    };

    class Field
    {
    protected:
        Rect range_;
    public:
        Field(const Rect& range);

        virtual void addToArgs(FieldArgs& args) const = 0;

        bool contains(Vector3d position);

        virtual void render() const = 0;
    };

    class ElectricField : public Field
    {
    private:
        Vector3d E_;   // N/C
    public:
        ElectricField(const Rect& range, Vector3d E);

        virtual void addToArgs(FieldArgs& args) const override;

        virtual void render() const override;
    };

    class MagneticField : public Field
    {
    private:
        Vector3d B_; // T
    public:
        MagneticField(const Rect& range, const Vector3d& B);

        virtual void addToArgs(FieldArgs& args) const override;

        virtual void render() const override;
    };

} // namespace simulation_app
