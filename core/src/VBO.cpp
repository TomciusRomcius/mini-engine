#include "VBO.h"

#include <numeric>
#include <GL/glew.h>

#include "Debugging.h"

namespace mini_engine {
    VBO::VBO(std::vector<AttributeMemory> attributes) {
        const GLint bytes = std::accumulate(attributes.begin(), attributes.end(), 0,
                                            [this](GLint sum, const AttributeMemory &m) {
                                                return sum + m.getSizeBytes();
                                            });
        glCall(glGenBuffers(1, &m_Vbo));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, m_Vbo));
        glCall(glBufferData(GL_ARRAY_BUFFER, bytes, nullptr, GL_STATIC_DRAW));

        GLint prevSize = 0;
        for (auto &attr: attributes) {
            auto bytes = attr.getData();
            glCall(glBufferSubData(GL_ARRAY_BUFFER, prevSize, 9 * sizeof(float), attr.getData()));
            prevSize = attr.getSizeBytes();
        }
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}
