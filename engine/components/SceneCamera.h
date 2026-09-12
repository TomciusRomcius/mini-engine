#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ecs/Component.h"

namespace mini_engine {
    class SceneCamera : public Component {
    public:
        float fovDegrees = 60.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        float moveSpeed = 5.0f;
        float mouseSensitivity = 0.1f;

        void setWindow(GLFWwindow *window);

        void start() override;
        void update() override;

        [[nodiscard]] glm::mat4 getViewMatrix() const;
        [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    private:
        [[nodiscard]] glm::vec3 front() const;
        [[nodiscard]] glm::vec3 right() const;

        GLFWwindow *m_Window = nullptr;
        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;
        double m_LastMouseX = 0.0;
        double m_LastMouseY = 0.0;
        bool m_FirstMouse = true;
        bool m_Rotating = false;
        double m_LastTime = 0.0;
    };
}
