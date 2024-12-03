#include "ResourcesPanel.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ResourcesPanel::ResourcesPanel() {
    currentPath = fs::current_path().string();
    pathHistory.push_back(currentPath); 

    folderIcon = LoadImage("Assets/Sprites/System/folderIcon.png");
    fileIcon = LoadImage("Assets/Sprites/System/folderIcon.png");

    if (!folderIcon || !fileIcon) {
        Log("Error: No se pudieron cargar los iconos.");
    }
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
            ImGui::Image(folderIcon, ImVec2(16, 16)); // Icono de carpeta
            ImGui::SameLine();
            if (ImGui::Selectable((name + "/").c_str(), false)) {
                NavigateTo(entry.path().string());
            }
        }
        else {
            ImGui::Image(fileIcon, ImVec2(16, 16)); // Icono de archivo
            ImGui::SameLine();
            ImGui::Text("%s", name.c_str());
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

// Método para cargar una imagen (puedes adaptarlo según tu backend)
ImTextureID ResourcesPanel::LoadImage(const std::string& path) {
    // Esto depende de tu implementación de backend de ImGui.
    // Aquí debes cargar la textura y devolver un ImTextureID.
    // Si usas OpenGL, por ejemplo, puedes usar stb_image.h para cargar la imagen
    // y crear una textura de OpenGL, devolviendo el ID de la textura.
    return nullptr; // Implementa la carga real aquí
}