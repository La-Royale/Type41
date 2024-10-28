#pragma once
#include "imgui.h"

class MainMenu {
public:
    void Render(bool& showConsole, bool& showConfig, bool& showHierarchy, bool& showInspector);
};
