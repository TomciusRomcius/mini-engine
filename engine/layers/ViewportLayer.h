#pragma once

#include <GL/glew.h>

#include "Layer.h"
#include "ecs/Scene.h"

namespace mini_engine {
    class ViewportLayer : public Layer {
    public:
        explicit ViewportLayer(Scene &scene);
        ~ViewportLayer() override;

        void onAttach() override;
        void update() override;

    private:
        void resize(int width, int height);
        void destroyFramebuffer();

        Scene &m_Scene;
        GLuint m_Fbo = 0;
        GLuint m_ColorTexture = 0;
        GLuint m_DepthRbo = 0;
        int m_Width = 0;
        int m_Height = 0;
    };
}
