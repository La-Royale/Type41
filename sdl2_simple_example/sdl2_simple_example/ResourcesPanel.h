#pragma once
#include "imgui.h"
#include "ConsolePanel.h"

class ResourcesPanel {
public:
    ResourcesPanel(ConsolePanel* consolePanel);
    ~ResourcesPanel();

    void Render(); // Renderiza el log de consola
    void Log(const char* message); // Método para registrar mensajes en consola

private:
    ConsolePanel* consolePanel;
    // Estructuras necesarias para el log
};
