#include "WindowEditor.h"
#include "SDL2/SDL.h"
#include "imgui.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ConfigPanel.h"
#include "Logger.h"

TimeManager timeManager;
ConsolePanel* consolePanel;
WindowEditor::WindowEditor()
    : consolePanel(new ConsolePanel()), configPanel(), hierarchyPanel(), inspectorPanel(), mainMenu(),
    showConsole(true), showConfig(true), showHierarchy(true), showInspector(true) {

    consolePanel = new ConsolePanel();
    configPanel = new ConfigPanel();
    hierarchyPanel = new HierarchyPanel();
    inspectorPanel = new InspectorPanel();
    mainMenu = new MainMenu();

    Logger::GetInstance().SetConsolePanel(consolePanel);

    // Ahora puedes loguear mensajes
    Logger::GetInstance().Log("Hello! The libraries on the project were all succesfuly added! You have now 2 game objects on scene!", INTRO);
}


WindowEditor::~WindowEditor() {
    delete consolePanel;
    delete configPanel;
    delete hierarchyPanel;
    delete inspectorPanel;
    delete mainMenu;
}

void WindowEditor::Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    timeManager.Update();
    float deltaTime = timeManager.GetDeltaTime();
    float fps = 1.0f / deltaTime;
    configPanel->UpdateFPS(fps);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    mainMenu->Render(showConsole, showConfig, showHierarchy, showInspector);

    if (showConsole) {
        consolePanel->Render();
    }
    if (showConfig) {
        configPanel->Render();
    }
    if (showHierarchy) {
        hierarchyPanel->Render(gameObjects);  // Pasamos el vector de GameObjects
    }

    if (showInspector) {
        inspectorPanel->Render();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void WindowEditor::HandleEvents() {
}
