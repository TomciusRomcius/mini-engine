#pragma once
#include <list>

#include "ECSContainer.h"
#include "Entity.h"

namespace mini_engine {
    class Scene {
    public:
        void addEntity(Entity *entity);
        Entity *createEntity();
        void removeEntity(Entity *entity);

        [[nodiscard]] const std::list<Entity *> &getEntities() const;

        void start();
        void update();

    private:
        std::list<Entity *> m_Entities;
        ECSContainer m_ECS;
    };
}
