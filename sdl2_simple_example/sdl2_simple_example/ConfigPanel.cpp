#include "ConfigPanel.h"
#include "imgui.h"
#include <SDL2/SDL.h>  // Asegúrate de incluir el header de SDL2
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

// Asegúrate de incluir el encabezado de Windows si estás usando funciones de memoria de Windows
#ifdef _WIN32
#include <windows.h>
#endif

// Constructor de ConfigPanel
ConfigPanel::ConfigPanel(MyWindow* window) : _window(window) {}
ConfigPanel::~ConfigPanel() {}
void ConfigPanel::UpdateFPS(float fps) {
    frameCount++;

    if (frameCount >= 10) {
        frameCount = 0;
        if (fpsHistory.size() >= maxSamples) {
            fpsHistory.erase(fpsHistory.begin());
        }
        fpsHistory.push_back(fps);
    }
}

void ConfigPanel::Render() {
    ImGui::SetNextWindowPos(ImVec2(946, 299));
    ImGui::SetNextWindowSize(ImVec2(334, 421));

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Mostrar el historial de FPS
    if (!fpsHistory.empty()) {
        ImGui::Text("Frames per Second (FPS)");
        ImGui::PlotLines("##FPS", fpsHistory.data(), fpsHistory.size(), 500, nullptr, 0.0f, 100.0f, ImVec2(200, 80));
    }
    else {
        ImGui::Text("No FPS data yet.");
    }

    // Información de versiones de software
    ImGui::Separator();
    ImGui::Text("Software Versions");
    ImGui::Text("SDL Version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
    ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));

    // Información de hardware
    ImGui::Separator();
    ImGui::Text("Hardware Information");
    ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
    ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
    ImGui::Text("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Información de memoria (en Windows como ejemplo)
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    ImGui::Text("Memory Usage: %.2f MB / %.2f MB", physMemUsed / (1024.0 * 1024.0), totalPhysMem / (1024.0 * 1024.0));
#else
    ImGui::Text("Memory information not available on this platform.");
#endif

    // Separador
    //ImGui::Separator();

    //// Agregar un combo-box para seleccionar la resolución
    //ImGui::Text("Change Window Resolution:");

    //const char* resolutions[] = {
    //    "1600x900 (ORIGINAL)",  // Resolución original
    //    "1280x720",  // Resolución 720p
    //    "1920x1080", // Resolución 1080p
    //    "2560x1440"  // Resolución 1440p
    //};

    //static int selectedResolution = 0; // Índice seleccionado en el combo-box

    //// Si la selección cambia, ajustamos el tamaño de la ventana
    //if (ImGui::Combo("##Resolution", &selectedResolution, resolutions, IM_ARRAYSIZE(resolutions))) {
    //    switch (selectedResolution) {
    //    case 0:
    //        SDL_SetWindowSize(_window->getWindow(), 1600, 900);  // 1600x900
    //        break;
    //    case 1:
    //        SDL_SetWindowSize(_window->getWindow(), 1280, 720);  // 1280x720
    //        break;
    //    case 2:
    //        SDL_SetWindowSize(_window->getWindow(), 1920, 1080); // 1920x1080
    //        break;
    //    case 3:
    //        SDL_SetWindowSize(_window->getWindow(), 2560, 1440); // 2560x1440
    //        break;
    //    default:
    //        break;
    //    }
    //}

    ImGui::End();
}

void ConfigPanel::Log(const char* message) {
    // Aquí podrías implementar la funcionalidad para loggear mensajes en el panel de configuración
}
