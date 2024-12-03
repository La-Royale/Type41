#pragma once
#include "imgui.h"
#include "MyWindow.h"
#include "HierarchyPanel.h"

class ScenePanel {
public:
    ScenePanel();
    ~ScenePanel();

    void Render(); // Renderiza el log de consola
    void Log(const char* message); // Método para registrar mensajes en consola

    // Método para establecer la textura del framebuffer
    void SetFramebufferTexture(unsigned int texture);

    void SetWindow(MyWindow* window);
    void SetHierarchyPanel(HierarchyPanel* hierarchyPanel);

private:
    // Estructuras necesarias para el log
    MyWindow* window = nullptr;         // Puntero al objeto MyWindow
    HierarchyPanel* hierarchy = nullptr; // Puntero al objeto HierarchyPanel
    unsigned int framebufferTexture; // Textura del framebuffer
};
