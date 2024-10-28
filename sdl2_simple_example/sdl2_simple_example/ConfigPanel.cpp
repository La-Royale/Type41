#include "ConfigPanel.h"
ConfigPanel::ConfigPanel() {}
ConfigPanel::~ConfigPanel() {}

void ConfigPanel::Render() {
    ImGui::Begin("Configuration");
    // Lógica de renderizado del log de consola
    ImGui::End();
}

void ConfigPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}
