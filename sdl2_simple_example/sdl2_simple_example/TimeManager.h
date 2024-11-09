#pragma once
#include <chrono>

class TimeManager {
public:
    TimeManager();
    void Update();           // Actualiza el tiempo cada frame
    float GetDeltaTime() const; // Devuelve el deltaTime en segundos

private:
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime;
};
