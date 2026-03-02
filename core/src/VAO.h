#pragma once
#include <vector>
#include <GL/glew.h>

#include "AttributeMemory.h"
#include "Debugging.h"

namespace mini_engine {
    class VAO {
    public:
        explicit VAO(const std::vector<AttributeMemory> &attributeLayouts) {
            glCall(glGenVertexArrays(1, &m_VAO));
            glCall(glBindVertexArray(m_VAO));
            size_t offset = 0;
            for (int i = 0; i < attributeLayouts.size(); i++) {
                glCall(
                    glVertexAttribPointer(i, 3, GL_FLOAT, attributeLayouts[i].isNormalized(), attributeLayouts[i].
                        getStride(),
                        reinterpret_cast<void *>(offset)));
                glCall(glEnableVertexAttribArray(0));
                offset += attributeLayouts[i].getSizeBytes();
            }
        }

        void bind() const {
            glCall(glBindVertexArray(m_VAO));
        }

    private:
        GLuint m_VAO = 0;
    };
} // mini_engine
