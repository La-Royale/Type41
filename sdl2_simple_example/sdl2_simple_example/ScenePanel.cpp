#include "ScenePanel.h"
ScenePanel::ScenePanel() {}
ScenePanel::~ScenePanel() {}

void ScenePanel::Render() {
    ImGui::Begin("Scene");
    // L�gica de renderizado del log de consola
    ImGui::End();
}

void ScenePanel::Log(const char* message) {
    // Implementaci�n para registrar un mensaje
}