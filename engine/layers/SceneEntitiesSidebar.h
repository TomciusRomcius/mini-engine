#pragma once

#include "Layer.h"
#include "ecs/Scene.h"

namespace mini_engine {
    class SceneEntitiesSidebar : public Layer {
    public:
        explicit SceneEntitiesSidebar(Scene &scene);

        void onAttach() override;
        void update() override;

    private:
        Scene &m_Scene;
    };
}
