#include "EBO.h"

#include <GL/glew.h>

#include "Debugging.h"

namespace mini_engine {
    EBO::EBO(const std::vector<uint32_t> &indices) {
        m_Count = static_cast<GLsizei>(indices.size());

        glCall(glGenBuffers(1, &m_Ebo));
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo));
        glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)),
                            indices.data(),
                            GL_STATIC_DRAW));
    }
}
