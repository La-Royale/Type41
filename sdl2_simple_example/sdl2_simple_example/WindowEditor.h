#pragma once

#include <vector>
#include "GL/glew.h"
#include "ConsolePanel.h"
#include "ConfigPanel.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "ScenePanel.h"
#include "ResourcesPanel.h"
#include "MainMenu.h"
#include "TimeManager.h"
#include "MyWindow.h"
#include "SimulationPanel.h"

class WindowEditor {
public:
    WindowEditor(HierarchyPanel& hierarchyPanel, MyWindow* window);
    ~WindowEditor();
    float inspectorHeight = 300.0f;
    void Render(std::vector<std::unique_ptr<GameObject>>& gameObjects);
    void HandleEvents();
    void SetFramebuffer(GLuint framebufferTexture);

    SimulationPanel* simulationPanel;

private:
    GLuint framebufferTexture;

    ConsolePanel* consolePanel;
    ConfigPanel* configPanel;
    HierarchyPanel& hierarchyPanel;
    InspectorPanel* inspectorPanel;
    ScenePanel* scenePanel;
    MainMenu* mainMenu;
    ResourcesPanel* resourcesPanel;

    bool showConsole;
    bool showConfig;
    bool showHierarchy;
    bool showInspector;
    bool showScene;
    bool showResources;
};
