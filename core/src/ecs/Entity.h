#pragma once
#include <cstdint>
#include <string>
#include <utility>

namespace mini_engine {
    using EntityId = uint64_t;

    class ECSContainer;

    class Entity {
    public:
        Entity() : m_Id(s_NextId++), m_Name("Entity " + std::to_string(m_Id)) {}

        explicit Entity(std::string name)
            : m_Id(s_NextId++),
              m_Name(std::move(name)) {}

        [[nodiscard]] EntityId getId() const {
            return m_Id;
        }

        [[nodiscard]] const std::string &getName() const {
            return m_Name;
        }

        void setName(std::string name) {
            m_Name = std::move(name);
        }

        void setECSContainer(ECSContainer *container) {
            m_ECS = container;
        }

        [[nodiscard]] ECSContainer *getECSContainer() const {
            return m_ECS;
        }

    private:
        EntityId m_Id;
        std::string m_Name;
        ECSContainer *m_ECS = nullptr;
        inline static EntityId s_NextId = 0;
    };
}
