#pragma once
#include <vector>
#include "MyWindow.h"

class ConfigPanel {

public:
    ConfigPanel(MyWindow* window);
    ~ConfigPanel();

    void UpdateFPS(float fps);
    void Render();
    void Log(const char* message);

private:
    MyWindow* _window;

    std::vector<float> fpsHistory;
    int frameCount = 0;
    static const int maxSamples = 100;
};
