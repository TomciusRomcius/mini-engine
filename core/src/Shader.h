#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <sstream>

namespace mini_engine {
    class Shader {
    public:
        Shader(GLenum type, const std::string &path);

        [[nodiscard]] inline GLint getId() const { return m_Shader; }

    private:
        static std::string loadShaderSource(const std::string &path);

        GLint m_Shader;
    };
} // mini_engine
