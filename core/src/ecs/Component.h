#pragma once

#include <any>
#include <string>
#include <type_traits>
#include <vector>

#include "Entity.h"
#include "Property.h"

namespace mini_engine {
    class PropertyBag {
    public:
        virtual ~PropertyBag() = default;

        void addProperty(
            std::string propName,
            std::string propType,
            IProperty::Getter getter = {},
            IProperty::Setter setter = {}) {
            for (IProperty &property: m_Properties) {
                if (property.getName() == propName) {
                    property = IProperty(std::move(propName), std::move(propType), std::move(getter), std::move(setter));
                    return;
                }
            }
            m_Properties.emplace_back(std::move(propName), std::move(propType), std::move(getter), std::move(setter));
        }

        [[nodiscard]] IProperty *getProperty(const std::string &propName) {
            for (IProperty &property: m_Properties) {
                if (property.getName() == propName) {
                    return &property;
                }
            }
            return nullptr;
        }

        [[nodiscard]] const IProperty *getProperty(const std::string &propName) const {
            for (const IProperty &property: m_Properties) {
                if (property.getName() == propName) {
                    return &property;
                }
            }
            return nullptr;
        }

        [[nodiscard]] const std::vector<IProperty> &getProperties() const {
            return m_Properties;
        }

    private:
        std::vector<IProperty> m_Properties;
    };

    class Component : public PropertyBag {
    public:
        ~Component() override = default;

        void SetEntity(Entity *entity) {
            m_Entity = entity;
        }

        [[nodiscard]] Entity *getEntity() const {
            return m_Entity;
        }

        virtual void start() {}
        virtual void update() {}

        [[nodiscard]] virtual const char *getTypeName() const {
            return "Component";
        }

        void addPropertyGroup(std::string name, std::vector<IProperty> properties) {
            removePropertyGroupIfExists(name);
            m_PropertyGroups.emplace_back(std::move(name), std::move(properties));
        }

        void removePropertyGroupIfExists(const std::string &name) {
            for (auto it = m_PropertyGroups.begin(); it != m_PropertyGroups.end(); ++it) {
                if (it->getName() == name) {
                    m_PropertyGroups.erase(it);
                    return;
                }
            }
        }

        [[nodiscard]] const std::vector<PropertyGroup> &getPropertyGroups() const {
            return m_PropertyGroups;
        }

        void addPropertyTabGroup(
            std::string name,
            std::vector<std::string> tabs,
            PropertyTabGroup::TabChangedCallback onTabChanged,
            PropertyTabGroup::CurrentTabGetter getCurrentTab) {
            for (PropertyTabGroup &tabGroup: m_PropertyTabGroups) {
                if (tabGroup.getName() == name) {
                    tabGroup = PropertyTabGroup(
                        std::move(name),
                        std::move(tabs),
                        std::move(onTabChanged),
                        std::move(getCurrentTab));
                    return;
                }
            }
            m_PropertyTabGroups.emplace_back(
                std::move(name),
                std::move(tabs),
                std::move(onTabChanged),
                std::move(getCurrentTab));
        }

        [[nodiscard]] const std::vector<PropertyTabGroup> &getPropertyTabGroups() const {
            return m_PropertyTabGroups;
        }

    private:
        Entity *m_Entity = nullptr;
        std::vector<PropertyGroup> m_PropertyGroups;
        std::vector<PropertyTabGroup> m_PropertyTabGroups;
    };
}

#define ME_ADD_PROPERTY(memberName, typeName)                                                         \
    addProperty(                                                                                      \
        #memberName,                                                                                  \
        typeName,                                                                                     \
        [this]() -> std::any {                                                                        \
            return this->memberName;                                                                  \
        },                                                                                            \
        [this](const std::any &value) {                                                               \
            using MemberT = std::decay_t<decltype(this->memberName)>;                                 \
            this->memberName = std::any_cast<MemberT>(value);                                         \
        })
