#include "ResourcesPanel.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ResourcesPanel::ResourcesPanel() {
    currentPath = fs::current_path().string();
    pathHistory.push_back(currentPath); 
}
ResourcesPanel::~ResourcesPanel() {}

void ResourcesPanel::Render() {
    ImGui::Begin("Resources");

    // Botón para subir un nivel en el árbol de directorios
    bool canGoUp = currentPath != pathHistory[0]; // Verifica si no estamos en la primera carpeta
    if (canGoUp && ImGui::Button("Up")) {
        std::string parentPath = fs::path(currentPath).parent_path().string();
        if (!parentPath.empty() && parentPath != currentPath) {
            NavigateTo(parentPath);
        }
    }

    // Mostrar los botones de navegación de la historia
    ImGui::SameLine();
    for (size_t i = 0; i < pathHistory.size(); ++i) {
        // Extraemos solo el nombre de la carpeta de la ruta completa
        std::string folderName = fs::path(pathHistory[i]).filename().string();

        if (ImGui::Button(folderName.c_str())) {
            NavigateTo(pathHistory[i]);
            break; // Salimos del bucle para evitar problemas al modificar la historia
        }
        ImGui::SameLine(); // Para que los botones estén en la misma línea
    }

    ImGui::Separator();

    // Mostrar el contenido de la carpeta actual
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        const std::string name = entry.path().filename().string(); // Obtiene solo el nombre

        if (entry.is_directory()) {
            // Mostrar carpetas
            if (ImGui::Selectable((name + "/").c_str(), false)) {
                NavigateTo(entry.path().string());
            }
        }
        else {
            // Mostrar archivos
            ImGui::Text(name.c_str());
        }
    }

    ImGui::End();
}

void ResourcesPanel::NavigateTo(const std::string& path) {
    currentPath = path;

    // Limpiamos la historia si navegamos hacia atrás
    auto it = std::find(pathHistory.begin(), pathHistory.end(), path);
    if (it != pathHistory.end()) {
        pathHistory.erase(it + 1, pathHistory.end());
    }
    else {
        // Agregar a la historia si es una nueva carpeta
        pathHistory.push_back(path);
    }

    Log(("Navegado a: " + path).c_str());
}

void ResourcesPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}