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

WindowEditor::WindowEditor(HierarchyPanel& hierarchyPanel, MyWindow* window)
    : consolePanel(new ConsolePanel()),
    configPanel(new ConfigPanel(window)),  // Pasamos 'window' a ConfigPanel
    hierarchyPanel(hierarchyPanel),
    inspectorPanel(new InspectorPanel()),
    scenePanel(new ScenePanel()),
    mainMenu(new MainMenu()),
    showConsole(true),
    showConfig(true),
    showHierarchy(true),
<<<<<<< Updated upstream
    showScene(true),
    showInspector(true) {
=======
    showInspector(true),
    showScene(true){
>>>>>>> Stashed changes

    Logger::GetInstance().SetConsolePanel(consolePanel);

    // Mensajes de log
    Logger::GetInstance().Log("Hello! The libraries on the project were all succesfuly added! You have now 2 game objects on scene!", INTRO);
    Logger::GetInstance().Log("MONTOTO WE LOVE YOU!!", INTRO);
}

WindowEditor::~WindowEditor() {
    delete consolePanel;
    delete configPanel;
    delete inspectorPanel;
    delete mainMenu;
    delete scenePanel;
}

void WindowEditor::Render(const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    timeManager.Update();
    float deltaTime = timeManager.GetDeltaTime();
    float fps = 1.0f / deltaTime;
    configPanel->UpdateFPS(fps);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    mainMenu->Render(showConsole, showConfig, showHierarchy, showInspector, showScene);

    if (showConsole) {
        consolePanel->Render();
    }
    if (showConfig) {
        configPanel->Render();
    }
    if (showHierarchy) {
        hierarchyPanel.Render(gameObjects);  // Pasamos el vector de GameObjects
    }

    if (showInspector) {
        inspectorPanel->Render();
    }

<<<<<<< Updated upstream
    if (showScene) {
=======
    if (showScene)
    {
>>>>>>> Stashed changes
        scenePanel->Render();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowEditor::HandleEvents() {
}
