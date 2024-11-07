#include "HierarchyPanel.h"
#include <iostream>

HierarchyPanel::HierarchyPanel() : selectedGameObject(nullptr) {}
HierarchyPanel::~HierarchyPanel() {}

void HierarchyPanel::Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    ImGui::Begin("Hierarchy");

    // Iterar sobre todos los GameObjects en la lista y mostrar su nombre
    for (const auto& gameObject : gameObjects) {
        const std::string& name = gameObject->getName();
        bool isSelected = (gameObject.get() == selectedGameObject);

        // Si se selecciona un GameObject, lo guardamos como seleccionado
        if (ImGui::Selectable(name.c_str(), isSelected)) {
            selectedGameObject = gameObject.get();
            std::cout << "HierarchyPanel -> Selected GameObject: " << selectedGameObject->getName() << std::endl;
        }
    }

    ImGui::End();

    // Mostrar detalles del GameObject seleccionado en un panel de "Inspector"
    if (selectedGameObject) {
        ImGui::Begin("Inspector");

        // Mostrar la posici�n con sliders
        glm::vec3 position = selectedGameObject->getPosition();
        if (ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f)) {
            selectedGameObject->setPosition(position);
        }

        // Mostrar rotaci�n con sliders
        glm::vec3 rotation = selectedGameObject->getRotation();
        if (ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f)) {
            selectedGameObject->setRotation(rotation);
        }

        // Mostrar escala con sliders
        glm::vec3 scale = selectedGameObject->getScale();
        if (ImGui::SliderFloat3("Scale", &scale.x, 0.1f, 10.0f)) {
            selectedGameObject->setScale(scale);
        }

        ImGui::End();
    }
}

