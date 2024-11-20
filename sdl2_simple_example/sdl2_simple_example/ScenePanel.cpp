#include "ScenePanel.h"
ScenePanel::ScenePanel() {}
ScenePanel::~ScenePanel() {}

void ScenePanel::Render() {
    ImGui::Begin("Scene");
    // Lógica de renderizado del log de consola
    ImGui::End();
}

void ScenePanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}