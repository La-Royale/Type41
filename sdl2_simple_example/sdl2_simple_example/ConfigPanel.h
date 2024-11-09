#pragma once
#include <vector>
#include "MyWindow.h"  // Incluimos el header de MyWindow

class ConfigPanel {

public:
    ConfigPanel(MyWindow* window);  // Constructor que recibe el puntero a MyWindow
    ~ConfigPanel();

    void UpdateFPS(float fps);
    void Render();
    void Log(const char* message);

private:
    MyWindow* _window;  // Puntero a la ventana MyWindow

    std::vector<float> fpsHistory;
    int frameCount = 0;
    static const int maxSamples = 100;
};
