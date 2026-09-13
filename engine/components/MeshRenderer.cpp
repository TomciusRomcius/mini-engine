#include "MeshRenderer.h"

#include <stdexcept>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "Debugging.h"
#include "Light.h"
#include "Mesh.h"
#include "SceneCamera.h"
#include "Transform.h"
#include "ecs/ECSContainer.h"
#include "ecs/Entity.h"

namespace mini_engine {
    namespace {
        constexpr int kMaxDirLights = 8;
        constexpr int kMaxPointLights = 8;
    }

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
        if (mesh.normals.size() != mesh.vertices.size()) {
            throw std::runtime_error("MeshRenderer requires a normal for each vertex");
        }

        AttributeMemory positions(
            mesh.vertices.data(),
            GL_FLOAT,
            static_cast<GLint>(mesh.vertices.size() * 3),
            3,
            false);

        AttributeMemory normalAttr(
            mesh.normals.data(),
            GL_FLOAT,
            static_cast<GLint>(mesh.normals.size() * 3),
            3,
            false);

        m_Vbo = std::make_unique<VBO>(std::vector<AttributeMemory>{positions, normalAttr});
        m_Vbo->bind();
        m_Vao = std::make_unique<VAO>(std::vector<AttributeMemory>{positions, normalAttr});
        m_Ebo = std::make_unique<EBO>(mesh.indices);

        const Shader vert(GL_VERTEX_SHADER, "shaders/3d.vert");
        const Shader frag(GL_FRAGMENT_SHADER, "shaders/3d.frag");
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
        SceneCamera *camera = ecs->findComponent<SceneCamera>();
        if (camera == nullptr || camera->getEntity() == nullptr) {
            return;
        }

        Transform &cameraTransform = ecs->getComponent<Transform>(camera->getEntity()->getId());

        const glm::mat4 model = transform.getModelMatrix();
        const glm::mat4 view = camera->getViewMatrix();
        const glm::mat4 proj = camera->getProjectionMatrix();

        glm::vec3 dirDirections[kMaxDirLights]{};
        glm::vec3 dirColors[kMaxDirLights]{};
        int dirCount = 0;

        glm::vec3 pointPositions[kMaxPointLights]{};
        glm::vec3 pointColors[kMaxPointLights]{};
        float pointRanges[kMaxPointLights]{};
        int pointCount = 0;

        ecs->forEachComponent<Light>([&](EntityId id, Light &lightComponent) {
            if (lightComponent.light == nullptr) {
                return;
            }

            const glm::vec3 radiance = lightComponent.color * lightComponent.intensity;

            if (auto *directional = dynamic_cast<DirectionalLight *>(lightComponent.light.get())) {
                if (dirCount >= kMaxDirLights) {
                    return;
                }
                dirDirections[dirCount] = directional->direction;
                dirColors[dirCount] = radiance;
                ++dirCount;
                return;
            }

            if (auto *point = dynamic_cast<PointLight *>(lightComponent.light.get())) {
                if (pointCount >= kMaxPointLights) {
                    return;
                }

                Transform *lightTransform = nullptr;
                try {
                    lightTransform = &ecs->getComponent<Transform>(id);
                } catch (const std::runtime_error &) {
                    return;
                }

                pointPositions[pointCount] = lightTransform->position;
                pointColors[pointCount] = radiance;
                pointRanges[pointCount] = point->range;
                ++pointCount;
            }
        });

        for (size_t i = 0; i < m_Textures.size(); ++i) {
            glCall(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i)));
            glCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]));
        }

        m_Program->bind();
        m_Program->setUniformMat4("uModel", glm::value_ptr(model));
        m_Program->setUniformMat4("uView", glm::value_ptr(view));
        m_Program->setUniformMat4("uProj", glm::value_ptr(proj));
        m_Program->setUniformVec3("uViewPos", glm::value_ptr(cameraTransform.position));

        m_Program->setUniformInt("uDirLightCount", dirCount);
        if (dirCount > 0) {
            m_Program->setUniformVec3Array("uDirLightDirections", glm::value_ptr(dirDirections[0]), dirCount);
            m_Program->setUniformVec3Array("uDirLightColors", glm::value_ptr(dirColors[0]), dirCount);
        }

        m_Program->setUniformInt("uPointLightCount", pointCount);
        if (pointCount > 0) {
            m_Program->setUniformVec3Array("uPointLightPositions", glm::value_ptr(pointPositions[0]), pointCount);
            m_Program->setUniformVec3Array("uPointLightColors", glm::value_ptr(pointColors[0]), pointCount);
            m_Program->setUniformFloatArray("uPointLightRanges", pointRanges, pointCount);
        }

        m_Vao->bind();
        m_Ebo->bind();
        glCall(glDrawElements(GL_TRIANGLES, m_Ebo->getCount(), GL_UNSIGNED_INT, nullptr));
    }
}
