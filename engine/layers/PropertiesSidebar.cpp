#include "PropertiesSidebar.h"

#include <any>
#include <string>

#include <imgui.h>
#include <glm/glm.hpp>

#include "ecs/Component.h"
#include "ecs/ECSContainer.h"
#include "ecs/Property.h"

namespace mini_engine {
    namespace {
        void drawProperty(const IProperty &property) {
            const std::string &type = property.getType();
            const std::string &name = property.getName();
            std::any value = property.getValue();

            if (type == "float") {
                float edited = std::any_cast<float>(value);
                if (ImGui::DragFloat(name.c_str(), &edited)) {
                    property.setValue(edited);
                }
                return;
            }

            if (type == "int") {
                int edited = std::any_cast<int>(value);
                if (ImGui::DragInt(name.c_str(), &edited)) {
                    property.setValue(edited);
                }
                return;
            }

            if (type == "bool") {
                bool edited = std::any_cast<bool>(value);
                if (ImGui::Checkbox(name.c_str(), &edited)) {
                    property.setValue(edited);
                }
                return;
            }

            if (type == "string") {
                std::string edited = std::any_cast<std::string>(value);
                char buffer[256] = {};
                const size_t copyCount = edited.size() < sizeof(buffer) - 1 ? edited.size() : sizeof(buffer) - 1;
                edited.copy(buffer, copyCount);
                if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) {
                    property.setValue(std::string(buffer));
                }
                return;
            }

            if (type == "vec3") {
                glm::vec3 edited = std::any_cast<glm::vec3>(value);
                if (ImGui::DragFloat3(name.c_str(), &edited.x)) {
                    property.setValue(edited);
                }
                return;
            }

            ImGui::Text("%s (%s)", name.c_str(), type.c_str());
        }
    }

    PropertiesSidebar::PropertiesSidebar(Entity *&selectedEntity)
        : m_SelectedEntity(selectedEntity) {}

    void PropertiesSidebar::onAttach() {}

    void PropertiesSidebar::update() {
        ImGui::Begin("Properties");

        if (m_SelectedEntity == nullptr) {
            ImGui::TextUnformatted("No entity selected");
            ImGui::End();
            return;
        }

        ECSContainer *ecs = m_SelectedEntity->getECSContainer();
        if (ecs == nullptr) {
            ImGui::TextUnformatted("Entity has no ECS container");
            ImGui::End();
            return;
        }

        ImGui::Text("Entity %llu", static_cast<unsigned long long>(m_SelectedEntity->getId()));
        ImGui::Separator();

        for (Component *component: ecs->getComponents(m_SelectedEntity->getId())) {
            if (component == nullptr) {
                continue;
            }

            if (ImGui::CollapsingHeader(component->getTypeName(), ImGuiTreeNodeFlags_DefaultOpen)) {
                const std::vector<IProperty> &properties = component->getProperties();
                if (properties.empty()) {
                    ImGui::TextDisabled("No exposed properties");
                } else {
                    for (const IProperty &property: properties) {
                        ImGui::PushID(property.getName().c_str());
                        drawProperty(property);
                        ImGui::PopID();
                    }
                }
            }
        }

        ImGui::End();
    }
}
