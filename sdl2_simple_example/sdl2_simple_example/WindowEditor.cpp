#include "WindowEditor.h"
#include "SDL2/SDL.h"
#include "imgui.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

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
    delete consolePanel;
    delete configPanel;
    delete hierarchyPanel;
    delete inspectorPanel;
    delete mainMenu;
}

void WindowEditor::Render() {
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
        ImGui::SetNextWindowPos(ImVec2(0, 20));
        ImGui::SetNextWindowSize(ImVec2(200, 580));
        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::End();
    }

    if (showInspector) {
        const float bottomFixedPosition = 900;

        ImGui::SetNextWindowSizeConstraints(ImVec2(1600, 200), ImVec2(1600, 400));
        ImGui::SetNextWindowSize(ImVec2(1600, inspectorHeight));

        ImVec2 windowPos(0, bottomFixedPosition - inspectorHeight);
        ImGui::SetNextWindowPos(windowPos);

        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_None);

        ImGui::SliderFloat(" ", &inspectorHeight, 200, 400);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowEditor::HandleEvents() {
}
