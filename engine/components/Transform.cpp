#include "Transform.h"

#include <any>

namespace mini_engine {
    namespace {
        const bool g_TransformPropertiesRegistered = [] {
            ME_ADD_PROPERTY(Transform, position, "vec3");
            ME_ADD_PROPERTY(Transform, scale, "vec3");
            ME_ADD_PROPERTY(Transform, rotation, "vec3");
            return true;
        }();
    }
}
