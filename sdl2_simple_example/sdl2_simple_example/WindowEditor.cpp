#include "WindowEditor.h"

WindowEditor::WindowEditor()
    : consolePanel(), configPanel(), hierarchyPanel(), inspectorPanel(), mainMenu(),
    showConsole(true), showConfig(true), showHierarchy(true), showInspector(true) {
    consolePanel = new ConsolePanel();
    configPanel = new ConfigPanel();
    hierarchyPanel = new HierarchyPanel();
    inspectorPanel = new InspectorPanel();
    mainMenu = new MainMenu();
    
}

WindowEditor::~WindowEditor() {
}

void WindowEditor::Render() {
    mainMenu->Render(showConsole, showConfig, showHierarchy, showInspector);
}

void WindowEditor::HandleEvents() {
}
