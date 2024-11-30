#include "ScenePanel.h"
ScenePanel::ScenePanel() {}
ScenePanel::~ScenePanel() {}

void ScenePanel::Render() {
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove); // Deshabilitar el movimiento de la ventana
    ImVec2 panelSize = ImGui::GetContentRegionAvail(); // Obtener el tamaño disponible del panel

    // Calcular el tamaño de la textura manteniendo la relación de aspecto
    float aspectRatio = panelSize.x / panelSize.y;
    float textureAspectRatio = 800.0f / 600.0f; // Cambia esto si tu textura tiene una relación de aspecto diferente

    ImVec2 textureSize;
    if (aspectRatio > textureAspectRatio) {
        textureSize.y = panelSize.y;
        textureSize.x = textureSize.y * textureAspectRatio;
    } else {
        textureSize.x = panelSize.x;
        textureSize.y = textureSize.x / textureAspectRatio;
    }

    if (framebufferTexture != 0) {
        ImGui::Image((void*)(intptr_t)framebufferTexture, textureSize, ImVec2(0, 1), ImVec2(1, 0)); // Ajustar el tamaño de la textura al tamaño del panel y voltear verticalmente
    } else {
        ImGui::Text("No framebuffer texture available.");
    }
    ImGui::End();
}

void ScenePanel::Log(const char* message) {
    // Implementaci�n para registrar un mensaje
}

void ScenePanel::SetFramebufferTexture(unsigned int texture) {
    framebufferTexture = texture;
}