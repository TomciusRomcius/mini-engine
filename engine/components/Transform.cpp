#include "Transform.h"

#include <any>

namespace mini_engine {
    namespace {
        const bool g_TransformPropertiesRegistered = [] {
            Component::addProperty<Transform>(
                "position",
                "vec3",
                [](Component *component) {
                    return static_cast<Transform *>(component)->position;
                },
                [](Component *component, const std::any &value) {
                    static_cast<Transform *>(component)->position = std::any_cast<glm::vec3>(value);
                });

            Component::addProperty<Transform>(
                "scale",
                "vec3",
                [](Component *component) {
                    return static_cast<Transform *>(component)->scale;
                },
                [](Component *component, const std::any &value) {
                    static_cast<Transform *>(component)->scale = std::any_cast<glm::vec3>(value);
                });

            Component::addProperty<Transform>(
                "rotation",
                "vec3",
                [](Component *component) {
                    return static_cast<Transform *>(component)->rotation;
                },
                [](Component *component, const std::any &value) {
                    static_cast<Transform *>(component)->rotation = std::any_cast<glm::vec3>(value);
                });

            return true;
        }();
    }
}
