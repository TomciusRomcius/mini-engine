#include "MeshRenderer.h"

#include <stdexcept>

#include "Debugging.h"
#include "Mesh.h"
#include "Transform.h"
#include "ecs/ECSContainer.h"
#include "ecs/Entity.h"

namespace mini_engine {
    void MeshRenderer::addTexture(GLuint texture) {
        m_Textures.push_back(texture);
    }

    const std::vector<GLuint> &MeshRenderer::getTextures() const {
        return m_Textures;
    }

    void MeshRenderer::start() {
        Entity *entity = getEntity();
        if (entity == nullptr || entity->getECSContainer() == nullptr) {
            throw std::runtime_error("MeshRenderer requires an entity with an ECS container");
        }

        ECSContainer *ecs = entity->getECSContainer();
        Mesh &mesh = ecs->getComponent<Mesh>(entity->getId());

        if (mesh.vertices.empty() || mesh.indices.empty()) {
            throw std::runtime_error("MeshRenderer requires a Mesh with vertices and indices");
        }

        AttributeMemory positions(
            mesh.vertices.data(),
            GL_FLOAT,
            static_cast<GLint>(mesh.vertices.size() * 3),
            3,
            false);

        m_Vbo = std::make_unique<VBO>(std::vector<AttributeMemory>{positions});
        m_Vbo->bind();
        m_Vao = std::make_unique<VAO>(std::vector<AttributeMemory>{positions});
        m_Ebo = std::make_unique<EBO>(mesh.indices);

        const Shader vert(GL_VERTEX_SHADER, "shaders/simple.vert");
        const Shader frag(GL_FRAGMENT_SHADER, "shaders/simple.frag");
        m_Program = std::make_unique<Program>(vert, frag);
    }

    void MeshRenderer::update() {
        Entity *entity = getEntity();
        if (entity == nullptr || entity->getECSContainer() == nullptr) {
            return;
        }

        if (!m_Vao || !m_Ebo || !m_Program) {
            return;
        }

        ECSContainer *ecs = entity->getECSContainer();
        Transform &transform = ecs->getComponent<Transform>(entity->getId());
        Mesh &mesh = ecs->getComponent<Mesh>(entity->getId());
        (void)transform;
        (void)mesh;

        for (size_t i = 0; i < m_Textures.size(); ++i) {
            glCall(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i)));
            glCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]));
        }

        m_Program->bind();
        m_Vao->bind();
        glCall(glDrawElements(GL_TRIANGLES, m_Ebo->getCount(), GL_UNSIGNED_INT, nullptr));
    }
}
