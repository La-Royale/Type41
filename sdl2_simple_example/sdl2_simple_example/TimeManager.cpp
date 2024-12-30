#include "TimeManager.h"

TimeManager::TimeManager()
    : deltaTime(0.0f), lastFrameTime(std::chrono::high_resolution_clock::now()) {
}

void TimeManager::Update() {
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime;
    deltaTime = elapsed.count();
    lastFrameTime = currentFrameTime; 
}

float TimeManager::GetDeltaTime() const {
    return deltaTime;
}
