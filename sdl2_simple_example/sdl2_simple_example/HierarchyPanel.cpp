#include "HierarchyPanel.h"
#include <iostream>

HierarchyPanel::HierarchyPanel()
    : selectedGameObject(nullptr), isRenaming(false), draggedObject(nullptr) {}

HierarchyPanel::~HierarchyPanel() {}

void HierarchyPanel::Render(std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    ImGui::Begin("Hierarchy");
    draggedObject = nullptr;
    static GameObject* dropTargetObject = nullptr;
    std::vector<std::unique_ptr<GameObject>>::iterator objectToDelete = gameObjects.end();

    static bool showRenamePopup = false;
    static char newNameBuffer[128] = "";

    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        GameObject* currentObject = it->get();

        if (currentObject->getParent() != nullptr) {
            continue;
        }

        const std::string& name = currentObject->getName();
        bool isSelected = (currentObject == selectedGameObject);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (currentObject->getChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        if (isSelected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags);

        if (ImGui::IsItemClicked()) {
            selectedGameObject = currentObject;
        }

        if (ImGui::BeginDragDropSource()) {
            if (currentObject->getParent()) {
                continue;
            }

            draggedObject = currentObject;
            ImGui::SetDragDropPayload("GAMEOBJECT", &draggedObject, sizeof(GameObject*));
            ImGui::Text("Arrastrando %s", draggedObject->getName().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT")) {
                if (payload->Data != nullptr) {
                    GameObject* droppedObject = *(GameObject**)payload->Data;

                    if (droppedObject && droppedObject != currentObject) {
                        auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                            [droppedObject](const std::unique_ptr<GameObject>& obj) {
                                return obj.get() == droppedObject;
                            });

                        if (it != gameObjects.end()) {
                            if (currentObject != nullptr) {
                                droppedObject->setParent(currentObject);
                            }
                        }
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }

        if (nodeOpen) {
            for (auto* child : currentObject->getChildren()) {
                RenderGameObject(child, gameObjects);
            }
            ImGui::TreePop();
        }

        if (ImGui::BeginPopupContextItem(name.c_str())) {
            if (ImGui::MenuItem("Copy")) {}

            if (ImGui::MenuItem("Paste")) {}

            if (ImGui::MenuItem("Duplicate")) {
                if (selectedGameObject) {
                    std::unique_ptr<GameObject> duplicatedObject = selectedGameObject->clone();
                    duplicatedObject->setName(selectedGameObject->getName() + "_Copy");
                    gameObjects.push_back(std::move(duplicatedObject));
                }
            }

            if (ImGui::MenuItem("Change Name")) {
                showRenamePopup = true;
                strncpy_s(newNameBuffer, name.c_str(), sizeof(newNameBuffer));
                newNameBuffer[sizeof(newNameBuffer) - 1] = '\0';
            }

            if (ImGui::MenuItem("Delete")) {
                objectToDelete = it;

                if (currentObject->getParent()) {
                    currentObject->getParent()->removeChild(currentObject);
                }
            }

            ImGui::EndPopup();
        }
    }

    if (objectToDelete != gameObjects.end()) {
        if (objectToDelete->get() == selectedGameObject) {
            selectedGameObject = nullptr;
        }

        gameObjects.erase(objectToDelete);
    }

    ImGui::End();

    if (showRenamePopup) {
        ImGui::OpenPopup("Rename GameObject");
        showRenamePopup = false;
    }

    if (ImGui::BeginPopupModal("Rename GameObject", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter a new name:");
        ImGui::InputText("##NewName", newNameBuffer, sizeof(newNameBuffer));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (selectedGameObject) {
                std::string newName = newNameBuffer;
                if (!newName.empty()) {
                    selectedGameObject->setName(newName);
                }
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (selectedGameObject) {
        RenderInspector(selectedGameObject);
    }
}


void HierarchyPanel::RenderGameObject(GameObject* gameObject, std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    const std::string& name = gameObject->getName();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (gameObject->getChildren().empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags);

    if (ImGui::IsItemClicked()) {
        selectedGameObject = gameObject;
    }

    if (nodeOpen) {
        for (auto* child : gameObject->getChildren()) {
            RenderGameObject(child, gameObjects);
        }
        ImGui::TreePop();
    }
}

void HierarchyPanel::RenderInspector(GameObject* selectedGameObject) {
    if (selectedGameObject) {
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

        ModelLoader& modelLoader = selectedGameObject->getModelLoader();
        const aiScene* scene = modelLoader.getScene();
        if (scene) {
            ImGui::Text("Mesh Information:");
            ImGui::Text("Number of Meshes: %d", scene->mNumMeshes);

            if (ImGui::Button("Show Triangle Normals")) {
                modelLoader.setShowTriangleNormals(!modelLoader.isShowingTriangleNormals());
            }

            if (ImGui::Button("Show Face Normals")) {
                modelLoader.setShowFaceNormals(!modelLoader.isShowingFaceNormals());
            }

            if (ImGui::Button("Show Bounding Box")) {
                modelLoader.setShowBoundingBox(!modelLoader.isShowingBoundingBox());
            }

            ImGui::Separator();

            bool isStatic = selectedGameObject->getStatic();
            if (ImGui::Checkbox("Is Static", &isStatic)) {
                selectedGameObject->setStatic(isStatic);
            }
        }
        ImGui::End();
    }
}

void HierarchyPanel::SetSelectedGameObject(GameObject* gameObject)
{
    selectedGameObject = gameObject;
}
