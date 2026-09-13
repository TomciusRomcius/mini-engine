#include "Transform.h"

#include <any>

namespace mini_engine {
    Transform::Transform() {
        ME_ADD_PROPERTY(position, "vec3");
        ME_ADD_PROPERTY(scale, "vec3");
        ME_ADD_PROPERTY(rotation, "vec3");
    }
}
