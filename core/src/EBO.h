#pragma once
#include <cstdint>
#include <vector>

#include "Debugging.h"

namespace mini_engine {
    class EBO {
    public:
        explicit EBO(const std::vector<uint32_t> &indices);

        ~EBO() {
            glCall(glDeleteBuffers(1, &m_Ebo));
        }

        inline void bind() const {
            glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo));
        }

        [[nodiscard]] inline uint32_t getId() const {
            return m_Ebo;
        }

        [[nodiscard]] inline GLsizei getCount() const {
            return m_Count;
        }

    private:
        uint32_t m_Ebo = 0;
        GLsizei m_Count = 0;
    };
}
