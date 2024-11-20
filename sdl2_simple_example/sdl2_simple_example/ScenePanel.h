#pragma once
#include "imgui.h"

class ScenePanel {
public:
    ScenePanel();
    ~ScenePanel();

    void Render(); // Renderiza el log de consola
    void Log(const char* message); // Método para registrar mensajes en consola

private:
    // Estructuras necesarias para el log
};
