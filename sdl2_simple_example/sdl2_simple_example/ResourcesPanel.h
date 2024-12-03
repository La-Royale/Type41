#pragma once
#include "imgui.h"
#include "ConsolePanel.h"

class ResourcesPanel {
public:
    ResourcesPanel();
    ~ResourcesPanel();

    void Render(); // Renderiza el log de consola
    void Log(const char* message); // M�todo para registrar mensajes en consola

private:
    std::string currentPath;

    std::vector<std::string> pathHistory; // Historia de carpetas para botones de navegaci�n

    void NavigateTo(const std::string& path); // M�todo para cambiar de carpeta
    // Estructuras necesarias para el log
};
