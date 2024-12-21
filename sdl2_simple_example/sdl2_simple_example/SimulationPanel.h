#pragma once

#include "imgui.h"

enum class SimulationState {
    STOPPED,
    RUNNING,
    PAUSED
};

class SimulationPanel {
public:
    SimulationPanel();
    void Render();
    SimulationState GetState() const;
    float GetElapsedTime() const;
    void UpdateElapsedTime(float deltaTime); 

private:
    SimulationState state;
    float elapsedTime; 
};
