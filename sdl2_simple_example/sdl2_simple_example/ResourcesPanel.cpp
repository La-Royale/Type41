#include "ResourcesPanel.h"

ResourcesPanel::ResourcesPanel(ConsolePanel* consolePanel): consolePanel(consolePanel) {}
ResourcesPanel::~ResourcesPanel() {}

void ResourcesPanel::Render() {
    ImGui::SetNextWindowPos(ImVec2(176, 519));
    ImGui::SetNextWindowSize(ImVec2(770, 200));

    ImGui::Begin("Resources", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    // Crear barra de tabs
    if (ImGui::BeginTabBar("ResourceTabs")) {
        // Primera pestaña
        if (ImGui::BeginTabItem("Tab 1")) {
            ImGui::Text("Contenido de la Tab 1");
            ImGui::EndTabItem();
        }

        // Segunda pestaña
        if (ImGui::BeginTabItem("Consola")) {
            if (consolePanel)
            {
                consolePanel->Render();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void ResourcesPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}