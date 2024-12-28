#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include <vector>
#include <memory>
#include "GameObject.h"
#include "imgui.h"
#include "CameraObject.h"

class HierarchyPanel {
public:
    HierarchyPanel();
    ~HierarchyPanel();

    // Renderiza la lista de GameObjects y maneja la selecci�n
    void Render(std::vector<std::unique_ptr<GameObject>>& gameObjects);
    void RenderGameObject(GameObject* gameObject, std::vector<std::unique_ptr<GameObject>>& gameObjects);
    void RenderInspector(GameObject* selectedGameObject);
    void SetSelectedGameObject(GameObject* gameObject);

    GameObject* getSelectedGameObject() const { return selectedGameObject; }
private:
    bool isRenaming;
    GameObject* draggedObject;
    GameObject* selectedGameObject;
};

#endif // HIERARCHYPANEL_H
