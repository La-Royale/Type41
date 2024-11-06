// HierarchyPanel.h
#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "imgui.h"

class HierarchyPanel {
public:
    HierarchyPanel();
    ~HierarchyPanel();

    // Modifica Render para aceptar la lista de GameObjects
    void Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects);

private:
    GameObject* selectedGameObject;
};
