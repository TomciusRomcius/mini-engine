#pragma once

#include <any>
#include <string>
#include <type_traits>
#include <vector>

#include "Entity.h"
#include "Property.h"

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

        [[nodiscard]] virtual const char *getTypeName() const {
            return "Component";
        }

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
        Entity *m_Entity = nullptr;
        std::vector<IProperty> m_Properties;
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
