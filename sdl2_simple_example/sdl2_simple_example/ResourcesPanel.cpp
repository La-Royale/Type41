#include "ResourcesPanel.h"

ResourcesPanel::ResourcesPanel(ConsolePanel* consolePanel): consolePanel(consolePanel) {}
ResourcesPanel::~ResourcesPanel() {}

void ResourcesPanel::Render() {
   
    ImGui::Begin("Resources");

    ImGui::End();
}

void ResourcesPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}