#include "ResourcesPanel.h"
#include <iostream>
#include <filesystem>
#include <imgui.h>

namespace fs = std::filesystem;

ResourcesPanel::ResourcesPanel() {
    basePath = fs::current_path().string();
    assetsPath = fs::path(basePath).append("Assets").string();
    libraryPath = fs::path(basePath).append("Library").string();
    currentPath = assetsPath;
    pathHistory.push_back(currentPath);
}
ResourcesPanel::~ResourcesPanel() {}

void ResourcesPanel::Render() {

    ImGui::Begin("Resources");

    if (ImGui::Button("Assets")) {
        NavigateTo(assetsPath);
    }
    ImGui::SameLine();

    if (ImGui::Button("Library")) {
        currentPath = libraryPath; 
    }

    ImGui::Separator();

    if (currentPath == libraryPath) {
        for (const auto& entry : fs::directory_iterator(basePath)) {
            if (entry.path() == assetsPath) continue; // Excluir Assets

            const std::string name = entry.path().filename().string();
            if (entry.is_directory()) {
                if (ImGui::Selectable(("> " + name).c_str(), false)) {
                    NavigateTo(entry.path().string());
                }
            }
            else {
                ImGui::Text("   < %s", name.c_str());
            }

            if (ImGui::BeginPopupContextItem(("ContextMenu_" + name).c_str())) {
                if (ImGui::MenuItem("Delete")) {
                    try {
                        fs::remove(entry.path());
                        Log(("Deleted: " + name).c_str());
                    }
                    catch (const std::exception& e) {
                        Log(("Failed to delete: " + name + ". Error: " + std::string(e.what())).c_str());
                    }
                }
                ImGui::EndPopup();
            }
        }
    }
    else {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            const std::string name = entry.path().filename().string();
            bool isDirectory = entry.is_directory();

            if (isDirectory) {
                if (ImGui::Selectable(("> " + name).c_str(), false)) {
                    NavigateTo(entry.path().string());
                }
            }
            else {
                ImGui::Text("   < %s", name.c_str());
            }

            if (ImGui::BeginPopupContextItem(("ContextMenu_" + name).c_str())) {
                if (ImGui::MenuItem("Delete")) {
                    try {
                        fs::remove(entry.path());
                        Log(("Deleted: " + name).c_str());
                    }
                    catch (const std::exception& e) {
                        Log(("Failed to delete: " + name + ". Error: " + std::string(e.what())).c_str());
                    }
                }
                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}

void ResourcesPanel::NavigateTo(const std::string& path) {
    currentPath = path;

    auto it = std::find(pathHistory.begin(), pathHistory.end(), path);
    if (it != pathHistory.end()) {
        pathHistory.erase(it + 1, pathHistory.end());
    }
    else {
        pathHistory.push_back(path);
    }
}

void ResourcesPanel::Log(const char* message) {
    //std::cout << message << std::endl;
}