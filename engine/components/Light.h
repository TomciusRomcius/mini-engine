#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "ecs/Component.h"

namespace mini_engine {
    class ILightType : public PropertyBag {
    public:
        ~ILightType() override = default;
    };

    class DirectionalLight : public ILightType {
    public:
        DirectionalLight();

        glm::vec3 direction{0.0f, -1.0f, 0.0f};
    };

    class PointLight : public ILightType {
    public:
        PointLight();

        float range = 10.0f;
    };

    class Light : public Component {
    public:
        Light();

        [[nodiscard]] const char *getTypeName() const override {
            return "Light";
        }

        glm::vec3 color{1.0f, 1.0f, 1.0f};
        float intensity = 1.0f;
        std::unique_ptr<ILightType> light;
        std::string lightType = "directional";
    };
}
