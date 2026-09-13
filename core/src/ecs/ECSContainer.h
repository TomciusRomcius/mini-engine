#pragma once
#include <list>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include "Component.h"
#include "Entity.h"

namespace mini_engine {
    class ECSContainer {
    public:
        void addComponent(Entity &entity, Component *component);
        void removeComponent(EntityId id, Component *component);
        void registerEntity(EntityId id);

        template<typename T>
        T &getComponent(EntityId id) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            const auto it = m_Components.find(id);
            if (it == m_Components.end()) {
                throw std::runtime_error("Entity not found in ECSContainer");
            }

            for (Component *component: it->second) {
                if (T *typed = dynamic_cast<T *>(component)) {
                    return *typed;
                }
            }

            throw std::runtime_error("Component type not found on entity");
        }

        template<typename T>
        T *findComponent() {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            for (auto &[id, components]: m_Components) {
                for (Component *component: components) {
                    if (T *typed = dynamic_cast<T *>(component)) {
                        return typed;
                    }
                }
            }

            return nullptr;
        }

        template<typename T, typename Fn>
        void forEachComponent(Fn &&fn) {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            for (auto &[id, components]: m_Components) {
                for (Component *component: components) {
                    if (T *typed = dynamic_cast<T *>(component)) {
                        fn(id, *typed);
                    }
                }
            }
        }

        [[nodiscard]] const std::list<Component *> &getComponents(EntityId id) const {
            static const std::list<Component *> empty;
            const auto it = m_Components.find(id);
            if (it == m_Components.end()) {
                return empty;
            }
            return it->second;
        }

    private:
        std::unordered_map<EntityId, std::list<Component *>> m_Components;
    };
}
