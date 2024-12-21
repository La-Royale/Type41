#include "SimulationPanel.h"

SimulationPanel::SimulationPanel() : state(SimulationState::STOPPED), elapsedTime(0.0f) {}

void SimulationPanel::Render() {
    ImGui::Begin("Simulation Control", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if (ImGui::Button("Start")) {
        state = SimulationState::RUNNING;
    }

    ImGui::SameLine();
    if (state == SimulationState::STOPPED) {
        ImGui::BeginDisabled();
        ImGui::Button("Pause");
        ImGui::EndDisabled();
    } else {
        if (ImGui::Button("Pause") && state == SimulationState::RUNNING) {
            state = SimulationState::PAUSED;
        }
    }

    ImGui::SameLine();
    if (state == SimulationState::STOPPED) {
        ImGui::BeginDisabled();
        ImGui::Button("Stop");
        ImGui::EndDisabled();
    } else {
        if (ImGui::Button("Stop")) {
            state = SimulationState::STOPPED;
            elapsedTime = 0.0f;
        }
    }

    ImGui::SameLine();
    ImGui::Text("%.2f seconds", elapsedTime);

    ImGui::End();
}

SimulationState SimulationPanel::GetState() const {
    return state;
}

float SimulationPanel::GetElapsedTime() const {
    return elapsedTime;
}

void SimulationPanel::UpdateElapsedTime(float deltaTime) {
    if (state == SimulationState::RUNNING) {
        elapsedTime += deltaTime;
    }
}
