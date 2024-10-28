#include "ConsolePanel.h"
ConsolePanel::ConsolePanel() {}
ConsolePanel::~ConsolePanel() {}

void ConsolePanel::Render() {
    ImGui::Begin("Console");
    // Lógica de renderizado del log de consola
    ImGui::End();
}

void ConsolePanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}
