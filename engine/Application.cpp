#include "Application.h"

#include <iostream>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Debugging.h"
#include "Program.h"
#include "VAO.h"

namespace mini_engine {
    Application::Application() {
        if (!glfwInit()) {
            throw std::runtime_error("GLFW initialization failed");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(1280, 720, "Mini engine", nullptr, nullptr);
        if (window == nullptr) {
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        GLenum err = glewInit();
        if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY) {
            throw std::runtime_error("glewInit() failed");
        }

        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };

        const AttributeMemory mem(vertices.data(), GL_FLOAT, vertices.size(), 3, false);
        const VBO vbo({mem});
        vbo.bind();
        const VAO vao({mem});
        const Shader vert(GL_VERTEX_SHADER, "shaders/simple.vert");
        const Shader frag(GL_FRAGMENT_SHADER, "shaders/simple.frag");
        const Program program(
            vert, frag
        );

        while (!glfwWindowShouldClose(window)) {
            glClearColor(1, 1, 0.8, 1);
            glCall(glClear(GL_COLOR_BUFFER_BIT));

            program.bind();
            vao.bind();

            glCall(glDrawArrays(GL_TRIANGLES, 0, 3));
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}
