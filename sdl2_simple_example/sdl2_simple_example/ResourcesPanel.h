#pragma once
#include "imgui.h"
#include "ConsolePanel.h"

typedef void* ImTextureID;

class ResourcesPanel {
public:
    ResourcesPanel();
    ~ResourcesPanel();

    void Render(); 
    void MoveContentToLibrary();
    void Log(const char* message); 

private:
    std::string basePath;
    std::string assetsPath;
    std::string libraryPath;
    std::string currentPath;

    std::vector<std::string> pathHistory;

    void NavigateTo(const std::string& path); 
};
