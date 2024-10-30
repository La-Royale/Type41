#include "TimeManager.h"

TimeManager::TimeManager()
    : deltaTime(0.0f), lastFrameTime(std::chrono::high_resolution_clock::now()) {
}

void TimeManager::Update() {
    // Calcula el deltaTime como la diferencia entre el tiempo actual y el último frame
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime;
    deltaTime = elapsed.count();
    lastFrameTime = currentFrameTime; // Actualiza el tiempo del último frame
}

float TimeManager::GetDeltaTime() const {
    return deltaTime;
}
