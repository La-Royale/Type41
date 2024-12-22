#include "HierarchyPanel.h"
#include <iostream>

HierarchyPanel::HierarchyPanel()
    : selectedGameObject(nullptr), isRenaming(false), draggedObject(nullptr) {
    // Aquí puedes inicializar cualquier otro miembro si es necesario
}

// Destructor de HierarchyPanel
HierarchyPanel::~HierarchyPanel() {
    // Si necesitas liberar recursos, hazlo aquí (aunque en este caso no parece necesario)
}

void HierarchyPanel::Render(std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    ImGui::Begin("Hierarchy");
    static GameObject* draggedObject = nullptr; // Referencia al objeto que estamos arrastrando
    static GameObject* dropTargetObject = nullptr; // Referencia al objeto donde soltaremos
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

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (currentObject->getChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        if (isSelected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags);

        // Si se selecciona un GameObject, lo guardamos como seleccionado
        if (ImGui::IsItemClicked()) {
            selectedGameObject = currentObject;
            std::cout << "HierarchyPanel -> Selected GameObject: " << selectedGameObject->getName() << std::endl;
        }

        // Iniciar arrastrar el objeto
        if (ImGui::BeginDragDropSource()) {
            // No arrastrar si el objeto ya tiene un padre
            if (currentObject->getParent()) {
                continue; // Salir de esta iteración si el objeto tiene un padre
            }

            draggedObject = currentObject;
            ImGui::SetDragDropPayload("GAMEOBJECT", &draggedObject, sizeof(GameObject*));  // Configuración del payload
            ImGui::Text("Arrastrando %s", draggedObject->getName().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            // Aceptamos el payload, que tiene el identificador "GAMEOBJECT"
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT")) {

                // Verificar si los datos del payload son válidos
                if (payload->Data != nullptr) {
                    // Convertir los datos de la carga útil al puntero correcto
                    GameObject* droppedObject = *(GameObject**)payload->Data;

                    // Verificar si el objeto arrastrado es válido y no es el objeto actual
                    if (droppedObject && droppedObject != currentObject) {
                        std::cout << "El objeto arrastrado es válido y no es el objeto actual." << std::endl;

                        // Verificar que el nombre del objeto no sea nulo o vacío
                        std::string objectName = droppedObject->getName();
                        if (objectName.empty()) {
                            std::cerr << "El nombre del objeto arrastrado está vacío." << std::endl;
                            return;
                        }

                        // Comprobar que la cadena esté terminada correctamente
                        if (objectName[objectName.size() - 1] != '\0') {
                            std::cerr << "El nombre del objeto no está correctamente terminado." << std::endl;
                            return;
                        }

                        // Verificar que los caracteres de la cadena sean válidos
                        for (size_t i = 0; i < objectName.size(); ++i) {
                            if (objectName[i] < 0 || objectName[i] > 127) {
                                std::cerr << "Carácter no válido en el nombre del objeto: " << objectName[i] << std::endl;
                                return;
                            }
                        }

                        // Asegúrate de que el objeto que estamos buscando está en la lista de objetos
                        auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                            [droppedObject](const std::unique_ptr<GameObject>& obj) {
                                return obj.get() == droppedObject;
                            });

                        if (it != gameObjects.end()) {
                            // Al eliminar, almacenamos el objeto eliminado para evitar su uso posterior
                            std::unique_ptr<GameObject> objectToDelete = std::move(*it);

                            // Eliminar el objeto de la lista antes de moverlo
                            gameObjects.erase(it);
                            std::cout << "Objeto eliminado de la lista." << std::endl;

                            // Asegúrate de que currentObject no es nulo antes de mover el objeto
                            if (currentObject != nullptr) {
                                objectToDelete->setParent(currentObject);
                                std::cout << "Moved " << objectToDelete->getName() << " under " << currentObject->getName() << std::endl;
                            }
                            else {
                                std::cerr << "currentObject es nulo." << std::endl;
                            }
                        }
                        else {
                            std::cerr << "No se encontró el objeto para eliminar." << std::endl;
                        }
                    }
                    else {
                        std::cerr << "El objeto arrastrado es nulo o es el mismo que el objeto actual." << std::endl;
                    }
                }
                else {
                    std::cerr << "El objeto arrastrado es nulo." << std::endl;
                }
            }
            else {
                std::cout << "No se aceptó la carga útil." << std::endl;
            }

            ImGui::EndDragDropTarget();
        }

        // Si el nodo está abierto, renderizar recursivamente los hijos
        if (nodeOpen) {
            for (auto* child : currentObject->getChildren()) {
                RenderGameObject(child, gameObjects);
            }
            ImGui::TreePop();
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
                if (selectedGameObject) {
                    // Crear una copia del objeto seleccionado utilizando el método clone
                    std::unique_ptr<GameObject> duplicatedObject = selectedGameObject->clone();

                    // Modificar el nombre del objeto duplicado para evitar duplicados en la jerarquía
                    duplicatedObject->setName(selectedGameObject->getName() + "_Copy");

                    // Añadir el objeto duplicado a la lista de objetos
                    gameObjects.push_back(std::move(duplicatedObject));
                }
            }

            if (ImGui::MenuItem("Change Name")) {
                showRenamePopup = true;
                strncpy_s(newNameBuffer, name.c_str(), sizeof(newNameBuffer)); // Copiar el nombre actual
                newNameBuffer[sizeof(newNameBuffer) - 1] = '\0'; // Asegurar el terminador nulo
            }

            if (ImGui::MenuItem("Delete")) {
                objectToDelete = it;

                // Si el objeto tiene padre, eliminarlo de la lista de hijos del padre
                if (currentObject->getParent()) {
                    currentObject->getParent()->removeChild(currentObject);
                }

                // Eliminar el objeto del vector
                gameObjects.erase(objectToDelete);
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
        RenderInspector(selectedGameObject);
    }
}

void HierarchyPanel::RenderInspector(GameObject* selectedGameObject) {
    // Implementación de la función RenderInspector
    if (selectedGameObject) {
        // Aquí deberías renderizar el inspector de propiedades del GameObject seleccionado.
        ImGui::Begin("Inspector");
        ImGui::Text("Selected GameObject: %s", selectedGameObject->getName().c_str());
        // Agrega más campos para editar las propiedades del GameObject
        ImGui::End();
    }
}

void HierarchyPanel::RenderGameObject(GameObject* gameObject, std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    // Implementación de la función RenderGameObject
    const std::string& name = gameObject->getName();
    ImGui::Text("%s", name.c_str());
    // Aquí puedes agregar más elementos para renderizar el GameObject y sus propiedades.
}