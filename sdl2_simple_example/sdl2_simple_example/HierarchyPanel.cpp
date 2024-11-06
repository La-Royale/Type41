#include "HierarchyPanel.h"

HierarchyPanel::HierarchyPanel() : selectedGameObject(nullptr) {}
HierarchyPanel::~HierarchyPanel() {}

void HierarchyPanel::Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    ImGui::Begin("Hierarchy");

    // Iterar sobre todos los GameObjects y mostrar sus nombres como elementos seleccionables
    for (const auto& gameObject : gameObjects) {
        // Obtén el nombre del GameObject
        const std::string& name = gameObject->getName();

        // Verifica si el objeto actual es el seleccionado
        bool isSelected = (gameObject.get() == selectedGameObject);

        // Crea un elemento seleccionable. Si se selecciona un GameObject, actualiza selectedGameObject
        if (ImGui::Selectable(name.c_str(), isSelected)) {
            selectedGameObject = gameObject.get();  // Actualiza la selección
        }
    }

    ImGui::End();

    // Mostrar detalles del GameObject seleccionado en un panel de "Inspector"
    if (selectedGameObject) {
        ImGui::Begin("Inspector");

        // Mostrar posición con controles interactivos
        glm::vec3 position = selectedGameObject->getPosition();
        if (ImGui::DragFloat3("Position", &position.x, 0.1f)) {
            selectedGameObject->setPosition(position);
        }

        // Mostrar rotación con controles interactivos
        glm::vec3 rotation = selectedGameObject->getRotation();
        if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) {
            selectedGameObject->setRotation(rotation);
        }

        // Mostrar escala con controles interactivos
        glm::vec3 scale = selectedGameObject->getScale();
        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) {
            selectedGameObject->setScale(scale);
        }

        ImGui::End();
    }
}
