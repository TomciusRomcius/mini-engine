#include "SceneEntitiesSidebar.h"

#include <imgui.h>

namespace mini_engine {
    SceneEntitiesSidebar::SceneEntitiesSidebar(Scene &scene) : m_Scene(scene) {}

    void SceneEntitiesSidebar::onAttach() {}

    void SceneEntitiesSidebar::update() {
        ImGui::Begin("Entities");

        for (Entity *entity: m_Scene.getEntities()) {
            ImGui::Selectable(("Entity " + std::to_string(entity->getId())).c_str());
        }

        ImGui::End();
    }
}
