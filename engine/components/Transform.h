#pragma once

#include <glm/glm.hpp>

#include "ecs/Component.h"

namespace mini_engine {
    class Transform : public Component {
    public:
        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };
}
