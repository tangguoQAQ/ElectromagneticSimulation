#pragma once

#include <memory>
#include <vector>
#include <thread>

#include "particle.h"
#include "field.h"

namespace simulation_app
{
    class Scene
    {
    private:
        std::vector<std::shared_ptr<Particle>> particles_;
        std::vector<std::shared_ptr<Field>> fields_;

        FieldArgs Scene::getFieldArgs(Vector3d position);

    public:
        void addParticle(std::shared_ptr<Particle> particle);

        void addField(std::shared_ptr<Field> field);
        
        void update(double dt);

        void dump();

        void render();
    };

    class SceneManager
    {
    private:
        const double dt_;    // s
        const int interval_seconds_;  // ms
        bool is_running_ = false;

        const std::shared_ptr<Scene> scene_;

        std::thread update_thread_;

        void updateThread();

    public:
        SceneManager(std::shared_ptr<Scene> scene, double dt = 1e-7, int interval_seconds = 50);

        inline std::shared_ptr<Scene> getScene() const
        {
            return scene_;
        }

        void start();

        void stop();

        void render();
    };

} // namespace simulation_app
