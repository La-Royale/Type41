#include "WindowEditor.h"

WindowEditor::WindowEditor()
    : consolePanel(), configPanel(), hierarchyPanel(), inspectorPanel(), mainMenu(),
    showConsole(true), showConfig(true), showHierarchy(true), showInspector(true) {
    // Constructor - inicializa los paneles y la visibilidad de cada uno
    consolePanel = new ConsolePanel();
    configPanel = new ConfigPanel();
    hierarchyPanel = new HierarchyPanel();
    inspectorPanel = new InspectorPanel();
    mainMenu = new MainMenu();
    
}

WindowEditor::~WindowEditor() {
    // Destructor - limpieza de recursos (si es necesario)
}

void WindowEditor::Render() {
    //// Llama al método Render de MainMenu pasando las referencias a las variables de visibilidad
    mainMenu->Render(showConsole, showConfig, showHierarchy, showInspector);

    //// Renderiza cada panel solo si su respectiva variable de visibilidad es true
    //if (showConsole) {
    //    consolePanel->Render();
    //}
    //if (showConfig) {
    //    configPanel->Render();
    //}
    //if (showHierarchy) {
    //    hierarchyPanel->Render();
    //}
    //if (showInspector) {
    //    inspectorPanel->Render();
    //}
}

void WindowEditor::HandleEvents() {
    // Manejo de eventos si es necesario en el futuro
}
