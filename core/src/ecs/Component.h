#pragma once

#include <string>
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

        [[nodiscard]] virtual const std::vector<IProperty> &exposedProperties() const {
            static const std::vector<IProperty> empty;
            return empty;
        }

        template<typename T>
        static void addProperty(
            std::string propName,
            std::string propType,
            IProperty::Getter getter = {},
            IProperty::Setter setter = {}) {
            std::vector<IProperty> &properties = propertyRegistry<T>();
            for (IProperty &property: properties) {
                if (property.getName() == propName) {
                    property = IProperty(std::move(propName), std::move(propType), std::move(getter), std::move(setter));
                    return;
                }
            }
            properties.emplace_back(std::move(propName), std::move(propType), std::move(getter), std::move(setter));
        }

        template<typename T>
        [[nodiscard]] static IProperty *getProperty(const std::string &propName) {
            for (IProperty &property: propertyRegistry<T>()) {
                if (property.getName() == propName) {
                    return &property;
                }
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] static const std::vector<IProperty> &getProperties() {
            return propertyRegistry<T>();
        }

    private:
        template<typename T>
        static std::vector<IProperty> &propertyRegistry() {
            static std::vector<IProperty> properties;
            return properties;
        }

        Entity *m_Entity = nullptr;
    };
}

#define ME_ADD_PROPERTY(ComponentType, memberName, typeName)                                          \
    ::mini_engine::Component::addProperty<ComponentType>(                                             \
        #memberName,                                                                                  \
        typeName,                                                                                     \
        [](::mini_engine::Component *component) {                                                     \
            return static_cast<ComponentType *>(component)->memberName;                               \
        },                                                                                            \
        [](::mini_engine::Component *component, const std::any &value) {                               \
            using MemberT = decltype(ComponentType::memberName);                                      \
            static_cast<ComponentType *>(component)->memberName = std::any_cast<MemberT>(value);      \
        })

