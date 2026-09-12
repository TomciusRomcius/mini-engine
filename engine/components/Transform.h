#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ecs/Component.h"

namespace mini_engine {
    class Transform : public Component {
    public:
        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};

        [[nodiscard]] const char *getTypeName() const override {
            return "Transform";
        }

        [[nodiscard]] const std::vector<IProperty> &exposedProperties() const override {
            return getProperties<Transform>();
        }

        [[nodiscard]] glm::mat4 getModelMatrix() const {
            glm::mat4 model(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, scale);
            return model;
        }
    };
}
