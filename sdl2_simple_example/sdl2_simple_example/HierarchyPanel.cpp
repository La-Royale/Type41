#include "HierarchyPanel.h"
HierarchyPanel::HierarchyPanel() {}
HierarchyPanel::~HierarchyPanel() {}

void HierarchyPanel::Render() {
    ImGui::Begin("Hierarchy");
    // Lógica de renderizado del log de consola
    ImGui::End();
}

void HierarchyPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}
