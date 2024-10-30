#include "ConfigPanel.h"
#include "imgui.h"

ConfigPanel::ConfigPanel() {}

ConfigPanel::~ConfigPanel() {}

void ConfigPanel::UpdateFPS(float fps) {
    frameCount++;

    if (frameCount >= 10) {
        frameCount = 0;
        if (fpsHistory.size() >= maxSamples) {
            fpsHistory.erase(fpsHistory.begin());
        }
        fpsHistory.push_back(fps);
    }
}

void ConfigPanel::Render() {
    ImGui::Begin("Configuration");

    if (!fpsHistory.empty()) {
        ImGui::Text("Frames per Second (FPS)");
        ImGui::PlotLines("##FPS", fpsHistory.data(), fpsHistory.size(), 500, nullptr, 0.0f, 100.0f, ImVec2(200, 80));
    }
    else {
        ImGui::Text("No FPS data yet.");
    }

    ImGui::End();
}

void ConfigPanel::Log(const char* message) {

}
