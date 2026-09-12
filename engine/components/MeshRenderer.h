#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>

#include "EBO.h"
#include "Program.h"
#include "VAO.h"
#include "VBO.h"
#include "ecs/Component.h"

namespace mini_engine {
    class MeshRenderer : public Component {
    public:
        void start() override;
        void update() override;

        void addTexture(GLuint texture);
        [[nodiscard]] const std::vector<GLuint> &getTextures() const;

        [[nodiscard]] const char *getTypeName() const override {
            return "MeshRenderer";
        }

    private:
        std::vector<GLuint> m_Textures;

        std::unique_ptr<VBO> m_Vbo;
        std::unique_ptr<VAO> m_Vao;
        std::unique_ptr<EBO> m_Ebo;
        std::unique_ptr<Program> m_Program;
    };
}
