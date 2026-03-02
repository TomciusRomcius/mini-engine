#pragma once
#include <vector>
#include <GL/glew.h>

namespace mini_engine {
    class AttributeMemory {
    public:
        AttributeMemory(void *data, GLenum type, GLint elementCount, GLint elementsPerVert, GLboolean normalized);

        [[nodiscard]] inline void *getData() const {
            return m_Data;
        }

        [[nodiscard]] inline GLenum getType() const {
            return m_Type;
        }

        [[nodiscard]] inline GLint getElementPerVert() const {
            return m_ElementPerVert;
        }

        [[nodiscard]] inline GLboolean isNormalized() const {
            return m_Normalized;
        }

        [[nodiscard]] inline GLint getSizeBytes() const {
            return m_SizeBytes;
        }

        [[nodiscard]] inline GLint getStride() const { return m_Stride; }

    private:
        void *m_Data;
        GLenum m_Type;
        GLint m_ElementPerVert;
        GLboolean m_Normalized;
        GLint m_SizeBytes;

        GLuint m_Stride;
    };
} // mini_engine
