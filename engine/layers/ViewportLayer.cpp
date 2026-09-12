#include "ViewportLayer.h"

#include <imgui.h>

#include "Debugging.h"
#include "components/SceneCamera.h"
#include "ecs/ECSContainer.h"
#include "ecs/Entity.h"

namespace mini_engine {
    ViewportLayer::ViewportLayer(Scene &scene) : m_Scene(scene) {}

    ViewportLayer::~ViewportLayer() {
        destroyFramebuffer();
    }

    void ViewportLayer::onAttach() {}

    void ViewportLayer::destroyFramebuffer() {
        if (m_ColorTexture != 0) {
            glDeleteTextures(1, &m_ColorTexture);
            m_ColorTexture = 0;
        }
        if (m_DepthRbo != 0) {
            glDeleteRenderbuffers(1, &m_DepthRbo);
            m_DepthRbo = 0;
        }
        if (m_Fbo != 0) {
            glDeleteFramebuffers(1, &m_Fbo);
            m_Fbo = 0;
        }
        m_Width = 0;
        m_Height = 0;
    }

    void ViewportLayer::resize(int width, int height) {
        if (width <= 0 || height <= 0) {
            return;
        }
        if (width == m_Width && height == m_Height) {
            return;
        }

        destroyFramebuffer();

        m_Width = width;
        m_Height = height;

        glCall(glGenFramebuffers(1, &m_Fbo));
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo));

        glCall(glGenTextures(1, &m_ColorTexture));
        glCall(glBindTexture(GL_TEXTURE_2D, m_ColorTexture));
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0));

        glCall(glGenRenderbuffers(1, &m_DepthRbo));
        glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRbo));
        glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
        glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRbo));

        glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void ViewportLayer::update() {
        ImGui::Begin("Viewport");

        const ImVec2 size = ImGui::GetContentRegionAvail();
        const int width = static_cast<int>(size.x);
        const int height = static_cast<int>(size.y);
        resize(width, height);

        for (Entity *entity: m_Scene.getEntities()) {
            ECSContainer *ecs = entity->getECSContainer();
            if (ecs == nullptr) {
                continue;
            }
            if (SceneCamera *camera = ecs->findComponent<SceneCamera>()) {
                camera->viewportWidth = m_Width;
                camera->viewportHeight = m_Height;
                break;
            }
        }

        if (m_Fbo != 0 && m_Width > 0 && m_Height > 0) {
            GLint previousViewport[4] = {};
            glGetIntegerv(GL_VIEWPORT, previousViewport);

            glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo));
            glCall(glViewport(0, 0, m_Width, m_Height));
            glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
            glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            m_Scene.update();

            glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            glCall(glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]));

            ImGui::Image(
                static_cast<ImTextureID>(static_cast<intptr_t>(m_ColorTexture)),
                size,
                ImVec2(0.0f, 1.0f),
                ImVec2(1.0f, 0.0f));
        }

        ImGui::End();
    }
}
