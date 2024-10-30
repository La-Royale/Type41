#pragma once
#include <vector>

class ConfigPanel {
public:
    ConfigPanel();
    ~ConfigPanel();

    void Render();
    void Log(const char* message);

    void UpdateFPS(float fps);

private:
    std::vector<float> fpsHistory;
    int frameCount = 0;
    static constexpr int maxSamples = 20;
};
