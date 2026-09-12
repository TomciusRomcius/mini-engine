#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "ecs/Component.h"

namespace mini_engine {
    class Mesh : public Component {
    public:
        std::vector<glm::vec3> vertices;
        std::vector<uint32_t> indices;
        std::vector<glm::vec2> uvs;

        [[nodiscard]] const char *getTypeName() const override {
            return "Mesh";
        }
    };
}
