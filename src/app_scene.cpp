#include <iostream>

#include "app_scene.h"
#include "scene.h"
#include "barrier.h"
#include "particle.h"
#include "field.h"
#include "rect.h"
#include "vector3d.h"
#include <memory>

namespace simulation_app
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    SceneManager sceneManager{scene, 0.002, 10};

    bool hasInitialized = false;

    void initializeScene()
    {
        if(hasInitialized)
        {
            return;
        }
        hasInitialized = true;
        
        // 加速电场
        scene->addField(std::make_shared<ElectricField>(
                Rect{15, 10, 35, 20}, Vector3d{0, -17, 0}));
        
        // 偏转磁场
        scene->addField(std::make_shared<MagneticField>(
                Rect{0, 22, 100, 50}, Vector3d{0, 0, 10}));
        
        scene->addBarrier(std::make_shared<Barrier>(
                Vector3d{0, 22, 0}, Vector3d{15, 22, 0}));

        scene->addParticle(std::make_shared<Particle>(
                Vector3d{25, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -1.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{26, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -2.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{27, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -3.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{28, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -4.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{29, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -5.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{30, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -6.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{31, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -7.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{32, 10.5, 0}, Vector3d{0, 0, 0}, 1.0, -8.0));
        scene->addParticle(std::make_shared<Particle>(
                Vector3d{28, 25, 0}, Vector3d{10, 0, 0}, 1.0, +1.0));

        sceneManager.start();
    }

    void uninitializeScene()
    {
        sceneManager.stop();
        hasInitialized = false;
    }

    void renderScene()
    {
        // scene->dump();
        sceneManager.render();
    }


} // namespace simulation_app
