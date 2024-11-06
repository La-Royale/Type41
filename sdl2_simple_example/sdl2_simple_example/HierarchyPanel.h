#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "imgui.h"

class HierarchyPanel {
public:
    HierarchyPanel();
    ~HierarchyPanel();

    // Renderiza la lista de GameObjects y maneja la selección
    void Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects);

private:
    GameObject* selectedGameObject;  // Puntero al GameObject actualmente seleccionado
};
