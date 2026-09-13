#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ecs/Component.h"

namespace mini_engine {
    class Transform : public Component {
    public:
        Transform();

        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};

        [[nodiscard]] const char *getTypeName() const override {
            return "Transform";
        }

        [[nodiscard]] glm::mat4 getModelMatrix() const {
            const glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
            const glm::mat4 rotateY =
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::mat4 rotateX =
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            const glm::mat4 rotateZ =
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            const glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
            return translate * rotateY * rotateX * rotateZ * scaling;
        }
    };
}
