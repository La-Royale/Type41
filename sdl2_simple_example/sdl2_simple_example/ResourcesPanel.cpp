#include "ResourcesPanel.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ResourcesPanel::ResourcesPanel() {
    basePath = fs::current_path().append("Assets").string();
    currentPath = basePath;
    pathHistory.push_back(basePath);
}
ResourcesPanel::~ResourcesPanel() {}

void ResourcesPanel::Render() {

    ImGui::Begin("Resources");

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
            if (ImGui::Selectable(("> " + name).c_str(), false)) {
                NavigateTo(entry.path().string());
            }
        }
        else {
            ImGui::Text("   < %s", name.c_str());
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
}

void ResourcesPanel::Log(const char* message) {
    // Implementación para registrar un mensaje
}