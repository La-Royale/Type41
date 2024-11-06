#include "HierarchyPanel.h"

HierarchyPanel::HierarchyPanel() {}
HierarchyPanel::~HierarchyPanel() {}

void HierarchyPanel::Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    ImGui::Begin("Hierarchy");
    for (const auto& gameObject : gameObjects) {
        // Puedes mostrar información de cada objeto, como su posición.
        auto position = gameObject->getPosition();
        ImGui::Text("GameObject: Posición (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
    ImGui::End();
}
