#include "ResourcesPanel.h"
ResourcesPanel::ResourcesPanel() {}
ResourcesPanel::~ResourcesPanel() {}

void ResourcesPanel::Render() {

    ImGui::SetNextWindowPos(ImVec2(176, 519));
    ImGui::SetNextWindowSize(ImVec2(770, 200));

    ImGui::Begin("Resources", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    // L�gica de renderizado del log de consola
    ImGui::End();
}

void ResourcesPanel::Log(const char* message) {
    // Implementaci�n para registrar un mensaje
}