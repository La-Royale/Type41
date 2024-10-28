#pragma once

#include <vector>
#include "ConsolePanel.h"
#include "ConfigPanel.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "MainMenu.h"

class WindowEditor {
public:
    WindowEditor();
    ~WindowEditor();

    void Render(); // Llama al renderizado de cada panel
    void HandleEvents(); // Manejo de eventos si es necesario

private:
    ConsolePanel* consolePanel;
    ConfigPanel* configPanel;
    HierarchyPanel* hierarchyPanel;
    InspectorPanel* inspectorPanel;
    MainMenu* mainMenu;

    // Variables para controlar la visibilidad de cada panel
    bool showConsole;
    bool showConfig;
    bool showHierarchy;
    bool showInspector;
};
