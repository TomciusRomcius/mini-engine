#pragma once
#include <cstdint>

namespace mini_engine {
    using EntityId = uint64_t;

    class ECSContainer;

    class Entity {
    public:
        Entity() : m_Id(s_NextId++) {}

        [[nodiscard]] EntityId getId() const {
            return m_Id;
        }

        void setECSContainer(ECSContainer *container) {
            m_ECS = container;
        }

        [[nodiscard]] ECSContainer *getECSContainer() const {
            return m_ECS;
        }

    private:
        EntityId m_Id;
        ECSContainer *m_ECS = nullptr;
        inline static EntityId s_NextId = 0;
    };
}
