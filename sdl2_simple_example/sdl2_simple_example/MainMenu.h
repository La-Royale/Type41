#pragma once
#include "imgui.h"
#include <vector>
#include <memory>
#include "GameObject.h"

extern std::vector<std::unique_ptr<GameObject>> gameObjects;

class MainMenu {
public:
    void Render(bool& showConsole, bool& showConfig, bool& showHierarchy, bool& showInspector);
};
