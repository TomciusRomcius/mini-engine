#include "SceneEntitiesSidebar.h"

#include <imgui.h>

namespace mini_engine {
    SceneEntitiesSidebar::SceneEntitiesSidebar(Scene &scene, Entity *&selectedEntity)
        : m_Scene(scene),
          m_SelectedEntity(selectedEntity) {}

    void SceneEntitiesSidebar::onAttach() {}

    void SceneEntitiesSidebar::update() {
        ImGui::Begin("Entities");

        for (Entity *entity: m_Scene.getEntities()) {
            const bool selected = m_SelectedEntity == entity;
            if (ImGui::Selectable(("Entity " + std::to_string(entity->getId())).c_str(), selected)) {
                m_SelectedEntity = entity;
            }
        }

        ImGui::End();
    }
}
