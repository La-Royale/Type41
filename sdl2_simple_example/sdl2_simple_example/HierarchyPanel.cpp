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

    if (selectedGameObject) {
        ImGui::Begin("Inspector");

        glm::vec3 position = selectedGameObject->getPosition();
        if (ImGui::DragFloat3("Position", &position.x, -0.1f, 0.1f)) {
            selectedGameObject->setPosition(position);
        }


        selectedGameObject->setPosition(position);

        glm::vec3 rotation = selectedGameObject->getRotation();
        if (ImGui::DragFloat3("Rotation", &rotation.x, -1.0f, 1.0f)) {
            selectedGameObject->setRotation(rotation);
        }

        selectedGameObject->setRotation(rotation);

        glm::vec3 scale = selectedGameObject->getScale();
        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 10.0f)) {
            selectedGameObject->setScale(scale);
        }

        selectedGameObject->setScale(scale);

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
        ModelLoader& modelLoader = selectedGameObject->getModelLoader(); // Ahora podemos acceder al ModelLoader
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

            else {
                ImGui::Text("No mesh loaded.");
            }

            ImGui::End();
        }
    }
}
