#include "InspectorPanel.h"
InspectorPanel::InspectorPanel() {}
InspectorPanel::~InspectorPanel() {}

void InspectorPanel::Render() {

    //ImGui::SetNextWindowPos(ImVec2(946, 100));
    //ImGui::SetNextWindowSize(ImVec2(334, 280));

    ImGui::Begin("Inspector"/*, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse*/);
    // L�gica de renderizado del log de consola
    ImGui::End();
}

void InspectorPanel::Log(const char* message) {
    // Implementaci�n para registrar un mensaje
}
