#include "ECSContainer.h"

#include <algorithm>

namespace mini_engine {
    void ECSContainer::addComponent(Entity &entity, Component *component) {
        if (component == nullptr) {
            throw std::runtime_error("Cannot add null component");
        }

        component->SetEntity(&entity);
        m_Components[entity.getId()].push_back(component);
    }

    void ECSContainer::registerEntity(EntityId id) {
        m_Components.try_emplace(id);
    }

    void ECSContainer::removeComponent(EntityId id, Component *component) {
        if (component == nullptr) {
            throw std::runtime_error("Cannot remove null component");
        }

        const auto it = m_Components.find(id);
        if (it == m_Components.end()) {
            throw std::runtime_error("Entity not found in ECSContainer");
        }

        auto &components = it->second;
        const auto compIt = std::find(components.begin(), components.end(), component);
        if (compIt == components.end()) {
            throw std::runtime_error("Component not found on entity");
        }

        components.erase(compIt);
    }
}
