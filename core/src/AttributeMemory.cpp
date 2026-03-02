#include "AttributeMemory.h"

#include <stdexcept>

namespace mini_engine {
    AttributeMemory::AttributeMemory(void *data, GLenum type, GLint elementCount, GLint elementsPerVert, GLboolean normalized) : m_Data(data), m_Type(type),
        m_ElementPerVert(elementsPerVert),
        m_Normalized(normalized) {
        uint32_t datatypeSize;
        if (m_Type == GL_FLOAT) {
            datatypeSize = sizeof(float);
            m_Stride = elementsPerVert * sizeof(GLfloat);
        }
        else if (m_Type == GL_UNSIGNED_INT) {
            datatypeSize = sizeof(unsigned int);
            m_Stride = elementsPerVert * sizeof(GLuint);
        }
        else throw std::runtime_error("unsupported type");
        m_SizeBytes = elementCount * datatypeSize;
    }
} // mini_engine
