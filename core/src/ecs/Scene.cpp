#include "Scene.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace mini_engine {
    void Scene::addEntity(Entity *entity) {
        if (entity == nullptr) {
            throw std::runtime_error("Cannot add null entity");
        }

        const auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
        if (it != m_Entities.end()) {
            throw std::runtime_error("Entity already exists in scene");
        }

        entity->setECSContainer(&m_ECS);
        m_Entities.push_back(entity);
    }

    Entity *Scene::createEntity() {
        Entity *entity = new Entity();
        addEntity(entity);
        m_ECS.registerEntity(entity->getId());
        return entity;
    }

    Entity *Scene::createEntity(std::string name) {
        Entity *entity = new Entity(std::move(name));
        addEntity(entity);
        m_ECS.registerEntity(entity->getId());
        return entity;
    }

    void Scene::removeEntity(Entity *entity) {
        if (entity == nullptr) {
            throw std::runtime_error("Cannot remove null entity");
        }

        const auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
        if (it == m_Entities.end()) {
            throw std::runtime_error("Entity not found in scene");
        }

        entity->setECSContainer(nullptr);
        m_Entities.erase(it);
    }

    const std::list<Entity *> &Scene::getEntities() const {
        return m_Entities;
    }

    void Scene::start() {
        for (Entity *entity: m_Entities) {
            for (Component *component: m_ECS.getComponents(entity->getId())) {
                component->start();
            }
        }
    }

    void Scene::update() {
        for (Entity *entity: m_Entities) {
            for (Component *component: m_ECS.getComponents(entity->getId())) {
                component->update();
            }
        }
    }
}
