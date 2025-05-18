#include <iostream>

#include "app_scene.h"
#include "scene.h"
#include "field.h"
#include "rect.h"
#include "vector3d.h"
#include <memory>

namespace simulation_app
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    SceneManager sceneManager{scene, 0.01};

    bool hasInitialized = false;

    void initializeScene()
    {
        if(hasInitialized)
        {
            return;
        }
        hasInitialized = true;

        // scene->addField(std::make_shared<MagneticField>(
        //         Rect{0, 0, 100, 100}, Vector3d{0, 0, 0.1}));
        // scene->addParticle(std::make_shared<Particle>(
        //         Vector3d{10, 10, 0}, Vector3d{0, 1e5, 0}, 1.67e-26, +1.6e-19));
        
        // 加速电场
        scene->addField(std::make_shared<ElectricField>(
                Rect{15, 10, 35, 20}, Vector3d{0, -7, 0}));
        
        // 偏转磁场
        scene->addField(std::make_shared<MagneticField>(
                Rect{0, 22, 100, 50}, Vector3d{0, 0, 1}));

        scene->addParticle(std::make_shared<Particle>(
                Vector3d{25, 10, 0}, Vector3d{0, 0, 0}, 1.0, -1.0));

        sceneManager.start();
    }

    void uninitializeScene()
    {
        sceneManager.stop();
    }

    void renderScene()
    {
        scene->dump();
        sceneManager.render();
    }


} // namespace simulation_app
