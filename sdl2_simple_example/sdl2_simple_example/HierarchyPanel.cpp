#include "HierarchyPanel.h"
#include <iostream>

HierarchyPanel::HierarchyPanel() : selectedGameObject(nullptr), isRenaming(false) {}
HierarchyPanel::~HierarchyPanel() {}

void HierarchyPanel::Render(std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    ImGui::Begin("Hierarchy");

    // Iterador para eliminación de objetos
    std::vector<std::unique_ptr<GameObject>>::iterator objectToDelete = gameObjects.end();

    // Variables para cambio de nombre
    static bool showRenamePopup = false; // Controla si el popup de renombrar está visible
    static char newNameBuffer[128] = ""; // Buffer para capturar el nuevo nombre

    // Iterar sobre todos los GameObjects en la lista y mostrar su nombre
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        GameObject* currentObject = it->get();
        const std::string& name = currentObject->getName();
        bool isSelected = (currentObject == selectedGameObject);

        // Si se selecciona un GameObject, lo guardamos como seleccionado
        if (ImGui::Selectable(name.c_str(), isSelected)) {
            selectedGameObject = currentObject;
            std::cout << "HierarchyPanel -> Selected GameObject: " << selectedGameObject->getName() << std::endl;
        }

        // Abrir menú contextual al hacer clic derecho sobre un objeto
        if (ImGui::BeginPopupContextItem(name.c_str())) {
            if (ImGui::MenuItem("Copy")) {
                std::cout << "Copy selected for: " << name << std::endl;
            }

            if (ImGui::MenuItem("Paste")) {
                std::cout << "Paste selected for: " << name << std::endl;
            }

            if (ImGui::MenuItem("Duplicate")) {
                std::cout << "Duplicate selected for: " << name << std::endl;
            }

            if (ImGui::MenuItem("Change Name")) {
                showRenamePopup = true;
                strncpy_s(newNameBuffer, name.c_str(), sizeof(newNameBuffer)); // Copiar el nombre actual
                newNameBuffer[sizeof(newNameBuffer) - 1] = '\0'; // Asegurar el terminador nulo
            }

            if (ImGui::MenuItem("Delete")) {
                objectToDelete = it;
            }
            ImGui::EndPopup();
        }
    }

    // Si hay un objeto marcado para eliminación, eliminarlo
    if (objectToDelete != gameObjects.end()) {
        // Si el objeto eliminado es el seleccionado, limpiar la selección
        if (objectToDelete->get() == selectedGameObject) {
            selectedGameObject = nullptr;
        }

        // Eliminar el objeto del vector
        gameObjects.erase(objectToDelete);
    }

    ImGui::End();

    // Si estamos en proceso de renombrar, mostrar el popup
    if (showRenamePopup) {
        ImGui::OpenPopup("Rename GameObject");
        showRenamePopup = false; // Resetear la bandera
    }

    if (ImGui::BeginPopupModal("Rename GameObject", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter a new name:");
        ImGui::InputText("##NewName", newNameBuffer, sizeof(newNameBuffer));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (selectedGameObject) {
                selectedGameObject->setName(newNameBuffer); // Asignar el nuevo nombre
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Si hay un objeto seleccionado, mostrar el Inspector de propiedades
    if (selectedGameObject) {
        // Inspector de propiedades del objeto seleccionado
        ImGui::Begin("Inspector");

        glm::vec3 position = selectedGameObject->getPosition();
        if (ImGui::DragFloat3("Position", &position.x, -0.1f, 0.1f)) {
            selectedGameObject->setPosition(position);
        }

        glm::vec3 rotation = selectedGameObject->getRotation();
        if (ImGui::DragFloat3("Rotation", &rotation.x, -1.0f, 1.0f)) {
            selectedGameObject->setRotation(rotation);
        }

        glm::vec3 scale = selectedGameObject->getScale();
        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 10.0f)) {
            selectedGameObject->setScale(scale);
        }

        ImGui::Separator();

        // Mostrar detalles de la textura (si tiene textura)
        Material& material = selectedGameObject->getMaterial();
        if (material.hasLoadedTexture()) {
            ImGui::Text("Texture Path: %s", material.getTexturePath().c_str());
            ImGui::Text("Width: %d", material.getTextureWidth());
            ImGui::Text("Height: %d", material.getTextureHeight());

            static bool showCheckeredTexture = false;
            if (ImGui::Checkbox("Show Checkered Texture", &showCheckeredTexture)) {
                if (showCheckeredTexture) {
                    unsigned int checkeredTexture = Material::generateCheckeredTexture(256, 256);
                    material.setTexture(checkeredTexture);
                }
                else {
                    material.loadTexture(material.getTexturePath());
                }
            }

            ImGui::Image((void*)material.getTextureID(), ImVec2(100, 100));
        }

        ImGui::Separator();

        // Mostrar información de la malla (si tiene malla)
        ModelLoader& modelLoader = selectedGameObject->getModelLoader();
        const aiScene* scene = modelLoader.getScene();
        if (scene) {
            ImGui::Text("Mesh Information:");
            ImGui::Text("Number of Meshes: %d", scene->mNumMeshes);

            if (ImGui::Button("Show Triangle Normals")) {
                modelLoader.setShowTriangleNormals(!modelLoader.isShowingTriangleNormals()); // Toggle
            }

            if (ImGui::Button("Show Face Normals")) {
                modelLoader.setShowFaceNormals(!modelLoader.isShowingFaceNormals()); // Toggle
            }

            ImGui::Separator();

            // Checkbox for isStatic
            bool isStatic = selectedGameObject->getStatic();
            if (ImGui::Checkbox("Is Static", &isStatic)) {
                selectedGameObject->setStatic(isStatic);
            }
        }
        else {
            ImGui::Text("No mesh loaded.");
        }

        ImGui::End();
    }
}
