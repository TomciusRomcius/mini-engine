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

        void setUniformMat4(const char *name, const float *matrix) const {
            const GLint location = glGetUniformLocation(m_Program, name);
            glCall(glUniformMatrix4fv(location, 1, GL_FALSE, matrix));
        }

        void setUniformInt(const char *name, int value) const {
            const GLint location = glGetUniformLocation(m_Program, name);
            glCall(glUniform1i(location, value));
        }

        void setUniformFloat(const char *name, float value) const {
            const GLint location = glGetUniformLocation(m_Program, name);
            glCall(glUniform1f(location, value));
        }

        void setUniformVec3(const char *name, const float *value) const {
            const GLint location = glGetUniformLocation(m_Program, name);
            glCall(glUniform3fv(location, 1, value));
        }

        void setUniformVec3Array(const char *name, const float *values, int count) const {
            const GLint location = glGetUniformLocation(m_Program, name);
            glCall(glUniform3fv(location, count, values));
        }

        void setUniformFloatArray(const char *name, const float *values, int count) const {
            const GLint location = glGetUniformLocation(m_Program, name);
            glCall(glUniform1fv(location, count, values));
        }

    private:
        GLint m_Program;
    };
}
