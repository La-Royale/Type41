#include "ScenePanel.h"
#include <iostream>
using namespace std;

ScenePanel::ScenePanel() {}
ScenePanel::~ScenePanel() {}

void ScenePanel::Render() {
    ImGui::Begin("Scene");

    isPanelFocused = ImGui::IsWindowFocused();
    isPanelHovered = ImGui::IsWindowHovered();

    ImVec2 panelSize = ImGui::GetContentRegionAvail(); 

    float aspectRatio = panelSize.x / panelSize.y;
    float textureAspectRatio = 980.0f / 600.0f;

    ImVec2 textureSize;
    if (aspectRatio > textureAspectRatio) {
        textureSize.y = panelSize.y;
        textureSize.x = textureSize.y * textureAspectRatio;
    } else {
        textureSize.x = panelSize.x;
        textureSize.y = textureSize.x / textureAspectRatio;
    }

    if (framebufferTexture != 0) {
        ImGui::Image((void*)(intptr_t)framebufferTexture, textureSize, ImVec2(0, 1), ImVec2(1, 0));
    } else {
        ImGui::Text("No framebuffer texture available.");
    }

    ImGui::End();
}

void ScenePanel::Log(const char* message) {
}

void ScenePanel::SetFramebufferTexture(unsigned int texture) {
    framebufferTexture = texture;
}