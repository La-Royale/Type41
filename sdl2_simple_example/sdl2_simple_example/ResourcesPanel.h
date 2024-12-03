#pragma once
#include "imgui.h"
#include "ConsolePanel.h"

typedef void* ImTextureID;

class ResourcesPanel {
public:
    ResourcesPanel();
    ~ResourcesPanel();

    void Render(); // Renderiza el log de consola
    void Log(const char* message); // Método para registrar mensajes en consola

private:
    std::string currentPath;

    std::vector<std::string> pathHistory; // Historia de carpetas para botones de navegación

    void NavigateTo(const std::string& path); // Método para cambiar de carpeta
    // Estructuras necesarias para el log

    ImTextureID folderIcon;       // Icono de carpeta
    ImTextureID fileIcon;         // Icono de archivo

    ImTextureID LoadImage(const std::string& path); // Método para cargar imágenes
};
