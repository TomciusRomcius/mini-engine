#pragma once

#include "Layer.h"
#include "ecs/Entity.h"

namespace mini_engine {
    class PropertiesSidebar : public Layer {
    public:
        explicit PropertiesSidebar(Entity *&selectedEntity);

        void onAttach() override;
        void update() override;

    private:
        Entity *&m_SelectedEntity;
    };
}
