//
// Created by tomas on 2/26/26.
//

#include "Shader.h"

#include "Debugging.h"

namespace mini_engine {
    Shader::Shader(GLenum type, const std::string &path) {
        m_Shader = glCall(glCreateShader(type));
        std::string source = loadShaderSource(path);
        const char* cSource = source.c_str();
        std::cout << source << std::endl;
        glCall(glShaderSource(m_Shader, 1, &cSource, nullptr));
        glCall(glCompileShader(m_Shader));
        GLint success;
        char infoLog[512];
        glCall(glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            glGetShaderInfoLog(m_Shader, 512, nullptr, infoLog);
            std::cout << "Shader compilation error in file " << path << '\n' << infoLog << '\n';
        }
    }

    std::string Shader::loadShaderSource(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "Failed to open " << path << std::endl;
            return "err";
        }
        std::ostringstream result;
        std::string line;
        while (std::getline(file, line)) {
            result << line << '\n';
        }

        return result.str();
    }
} // mini_engine