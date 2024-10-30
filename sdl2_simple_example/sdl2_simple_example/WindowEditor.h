#pragma once

#include <vector>
#include "ConsolePanel.h"
#include "ConfigPanel.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "MainMenu.h"
#include "TimeManager.h"

class WindowEditor {
public:
    WindowEditor();
    ~WindowEditor();
    float inspectorHeight = 300.0f;
    void Render();
    void HandleEvents();

private:
    ConsolePanel* consolePanel;
    ConfigPanel* configPanel;
    HierarchyPanel* hierarchyPanel;
    InspectorPanel* inspectorPanel;
    MainMenu* mainMenu;

    bool showConsole;
    bool showConfig;
    bool showHierarchy;
    bool showInspector;
};
