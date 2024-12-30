#pragma once
#include <chrono>

class TimeManager {
public:
    TimeManager();
    void Update();           
    float GetDeltaTime() const; 

private:
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime;
};
