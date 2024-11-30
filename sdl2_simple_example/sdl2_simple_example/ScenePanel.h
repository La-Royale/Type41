#pragma once
#include "imgui.h"

class ScenePanel {
public:
    ScenePanel();
    ~ScenePanel();

    void Render(); // Renderiza el log de consola
    void Log(const char* message); // Método para registrar mensajes en consola

    // Método para establecer la textura del framebuffer
    void SetFramebufferTexture(unsigned int texture);

private:
    // Estructuras necesarias para el log
    unsigned int framebufferTexture; // Textura del framebuffer
};
