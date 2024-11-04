#include "ConfigPanel.h"
#include "imgui.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <IL/il.h> // DevIL
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h> // Para GlobalMemoryStatusEx
#endif

ConfigPanel::ConfigPanel() {}

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
    ImGui::Begin("Configuration");

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
    ImGui::Text("DevIL Version: %d", IL_VERSION);

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

    ImGui::End();
}

void ConfigPanel::Log(const char* message) {
    // Aquí podrías implementar la funcionalidad para loggear mensajes en el panel de configuración
}
