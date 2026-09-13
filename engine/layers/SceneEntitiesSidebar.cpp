#include "SceneEntitiesSidebar.h"

#include <string>

#include <imgui.h>

namespace mini_engine {
    SceneEntitiesSidebar::SceneEntitiesSidebar(Scene &scene, Entity *&selectedEntity)
        : m_Scene(scene),
          m_SelectedEntity(selectedEntity),
          m_EntityName{"New Entity"} {}

    void SceneEntitiesSidebar::onAttach() {}

    void SceneEntitiesSidebar::update() {
        ImGui::Begin("Entities");

        for (Entity *entity: m_Scene.getEntities()) {
            const bool selected = m_SelectedEntity == entity;
            ImGui::PushID(static_cast<int>(entity->getId()));
            if (ImGui::Selectable(entity->getName().c_str(), selected)) {
                m_SelectedEntity = entity;
            }
            ImGui::PopID();
        }

        ImGui::Separator();
        ImGui::InputText("Name", m_EntityName, kEntityNameMaxSize);
        if (ImGui::Button("Create Entity")) {
            std::string name = m_EntityName;
            if (name.empty()) {
                name = "New Entity";
            }
            m_SelectedEntity = m_Scene.createEntity(std::move(name));
        }

        ImGui::End();
    }
}
