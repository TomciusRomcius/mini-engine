#include "Light.h"

#include <any>

namespace mini_engine {
    DirectionalLight::DirectionalLight() {
        ME_ADD_PROPERTY(direction, "vec3");
    }

    PointLight::PointLight() {
        ME_ADD_PROPERTY(range, "float");
    }

    Light::Light() {
        ME_ADD_PROPERTY(color, "vec3");
        ME_ADD_PROPERTY(intensity, "float");

        light = std::make_unique<DirectionalLight>();
        addPropertyGroup("light", light->getProperties());

        addPropertyTabGroup(
            "type",
            {"directional", "point"},
            [this](const std::string &tab) {
                removePropertyGroupIfExists("light");
                lightType = tab;
                if (tab == "directional") {
                    light = std::make_unique<DirectionalLight>();
                } else if (tab == "point") {
                    light = std::make_unique<PointLight>();
                }
                if (light != nullptr) {
                    addPropertyGroup("light", light->getProperties());
                }
            },
            [this]() {
                return lightType;
            });
    }
}
