#pragma once
#include "Debugging.h"
#include "Shader.h"

namespace mini_engine {
    class Program {
    public:
        Program(const Shader& vertexShader, const Shader& fragmentShader) {
            m_Program = glCall(glCreateProgram());
            glCall(glAttachShader(m_Program, vertexShader.getId()));
            glCall(glAttachShader(m_Program, fragmentShader.getId()));
            glCall(glLinkProgram(m_Program));

            GLint success;
            glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
            if (!success) {
                std::cout << "Failed to link shader program" << std::endl;
            }
        }

        void bind() const {
            glCall(glUseProgram(m_Program));
        }

    private:
        GLint m_Program;
    };
}
