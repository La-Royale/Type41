#include "InspectorPanel.h"
InspectorPanel::InspectorPanel() {}
InspectorPanel::~InspectorPanel() {}

void InspectorPanel::Render() {
    ImGui::Begin("Inspector");
    // Lógica de renderizado del log de consola
    ImGui::End();
}

void InspectorPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}
