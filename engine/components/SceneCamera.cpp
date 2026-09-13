#include "SceneCamera.h"

#include <algorithm>
#include <any>
#include <cmath>
#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"
#include "ecs/ECSContainer.h"
#include "ecs/Entity.h"

namespace mini_engine {
    SceneCamera::SceneCamera() {
        ME_ADD_PROPERTY(fovDegrees, "float");
        ME_ADD_PROPERTY(nearPlane, "float");
        ME_ADD_PROPERTY(farPlane, "float");
        ME_ADD_PROPERTY(moveSpeed, "float");
        ME_ADD_PROPERTY(mouseSensitivity, "float");
    }

    void SceneCamera::setWindow(GLFWwindow *window) {
        m_Window = window;
    }

    void SceneCamera::start() {
        m_LastTime = glfwGetTime();
    }

    glm::vec3 SceneCamera::front() const {
        const float yawRad = glm::radians(m_Yaw);
        const float pitchRad = glm::radians(m_Pitch);

        glm::vec3 direction;
        direction.x = std::cos(yawRad) * std::cos(pitchRad);
        direction.y = std::sin(pitchRad);
        direction.z = std::sin(yawRad) * std::cos(pitchRad);
        return glm::normalize(direction);
    }

    glm::vec3 SceneCamera::right() const {
        return glm::normalize(glm::cross(front(), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    glm::mat4 SceneCamera::getViewMatrix() const {
        Entity *entity = getEntity();
        ECSContainer *ecs = entity->getECSContainer();
        const Transform &transform = ecs->getComponent<Transform>(entity->getId());

        const glm::vec3 eye = transform.position;
        return glm::lookAt(eye, eye + front(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 SceneCamera::getProjectionMatrix() const {
        const float aspect = viewportHeight > 0
            ? static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight)
            : 1.0f;

        return glm::perspective(glm::radians(fovDegrees), aspect, nearPlane, farPlane);
    }

    void SceneCamera::update() {
        if (m_Window == nullptr) {
            return;
        }

        Entity *entity = getEntity();
        if (entity == nullptr || entity->getECSContainer() == nullptr) {
            return;
        }

        ECSContainer *ecs = entity->getECSContainer();
        Transform &transform = ecs->getComponent<Transform>(entity->getId());

        const double now = glfwGetTime();
        const float deltaTime = static_cast<float>(now - m_LastTime);
        m_LastTime = now;

        const bool rotating = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        if (rotating && !m_Rotating) {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_FirstMouse = true;
        } else if (!rotating && m_Rotating) {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        m_Rotating = rotating;

        if (rotating) {
            double mouseX = 0.0;
            double mouseY = 0.0;
            glfwGetCursorPos(m_Window, &mouseX, &mouseY);

            if (m_FirstMouse) {
                m_LastMouseX = mouseX;
                m_LastMouseY = mouseY;
                m_FirstMouse = false;
            }

            const float offsetX = static_cast<float>(mouseX - m_LastMouseX);
            const float offsetY = static_cast<float>(m_LastMouseY - mouseY);
            m_LastMouseX = mouseX;
            m_LastMouseY = mouseY;

            m_Yaw += offsetX * mouseSensitivity;
            m_Pitch += offsetY * mouseSensitivity;
            m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

            transform.rotation = glm::vec3(m_Pitch, m_Yaw, 0.0f);
        }

        const float velocity = moveSpeed * deltaTime;
        const glm::vec3 camFront = front();
        const glm::vec3 camRight = right();

        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
            transform.position += camFront * velocity;
        }
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
            transform.position -= camFront * velocity;
        }
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
            transform.position -= camRight * velocity;
        }
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
            transform.position += camRight * velocity;
        }
    }
}
