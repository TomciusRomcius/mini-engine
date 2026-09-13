#pragma once

#include "Layer.h"
#include "ecs/Entity.h"
#include "ecs/Scene.h"

namespace mini_engine {
    class SceneEntitiesSidebar : public Layer {
    public:
        SceneEntitiesSidebar(Scene &scene, Entity *&selectedEntity);

        void onAttach() override;
        void update() override;

    private:
        static constexpr size_t kEntityNameMaxSize = 64;

        Scene &m_Scene;
        Entity *&m_SelectedEntity;
        char m_EntityName[kEntityNameMaxSize]{};
    };
}
