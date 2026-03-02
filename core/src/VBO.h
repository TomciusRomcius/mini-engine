#pragma once
#include <cstdint>
#include <vector>

#include "AttributeMemory.h"
#include "Debugging.h"

namespace mini_engine {
    class VBO {
    public:
        explicit VBO(std::vector<mini_engine::AttributeMemory> attributes);

        inline void bind() const {
            glCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
        };

    private:
        uint32_t m_Vbo;
    };
}
