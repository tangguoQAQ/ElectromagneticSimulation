#include "scene.h"
#include "field.h"
#include <memory>   // include 顺序错误会导致模板类型错误
#include <vector>
#include <thread>
#include <iostream>

namespace simulation_app
{
    void Scene::addParticle(std::shared_ptr<Particle> particle)
    {
        particles_.push_back(particle);
    }

    void Scene::addField(std::shared_ptr<Field> field)
    {
        fields_.push_back(field);
    }

    FieldArgs Scene::getFieldArgs(Vector3d position)
    {
        FieldArgs result{};

        for(auto& field : fields_)
        {
            if(field->contains(position))
            {
                field->addToArgs(result);
            }
        }

        return result;
    } 
    
    void Scene::update(double dt)
    {
        for(auto& particle : particles_)
        {
            particle->update(dt, getFieldArgs(particle->getPos()));
        }
    }

    void Scene::dump()
    {
        std::cout << "Scene dump:" << std::endl;
        for(auto& particle : particles_)
        {
            std::cout << "Particle: " << &particle << " pos: "
                    << particle->getPos().x << " " << particle->getPos().y << " " << particle->getPos().z << std::endl;
        }
    }

    void Scene::render()
    {
        for(auto& field : fields_)
        {
            field->render();
        }
        for(auto& particle : particles_)
        {
            particle->render();
        }
    }

    void SceneManager::updateThread()
    {
        while(is_running_)
        {
            scene_->update(dt_);

            std::this_thread::sleep_for(std::chrono::milliseconds(interval_seconds_));
        }
    }

    SceneManager::SceneManager(std::shared_ptr<Scene> scene, double dt, int interval_seconds)
        : scene_(scene), dt_(dt), interval_seconds_(interval_seconds)
    {
    }

    void SceneManager::start()
    {
        is_running_ = true;
        update_thread_ = std::move(std::thread(&SceneManager::updateThread, this));
    }

    void SceneManager::stop()
    {
        is_running_ = false;
        update_thread_.join();
    }

    void SceneManager::render()
    {
        scene_->render();
    }

}
