#pragma once

#include "Entity.h"

namespace mini_engine {
    class Component {
    public:
        virtual ~Component() = default;

        void SetEntity(Entity *entity) {
            m_Entity = entity;
        }

        [[nodiscard]] Entity *getEntity() const {
            return m_Entity;
        }

        virtual void start() {}
        virtual void update() {}

    private:
        Entity *m_Entity = nullptr;
    };
}
