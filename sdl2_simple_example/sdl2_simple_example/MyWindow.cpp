#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "MyWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include "GameObject.h"
#include <memory>
#include <vector>
#include <iostream>
#include "Logger.h"

extern std::vector<std::unique_ptr<GameObject>> gameObjects;

using namespace std;

MyWindow::MyWindow(const std::string& title, int w, int h) : _width(w), _height(h) {

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);
    if (!_window) throw exception(SDL_GetError());

    _ctx = SDL_GL_CreateContext(_window);
    if (!_ctx) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());

    ImGui::CreateContext();

    g_io = &ImGui::GetIO();
    g_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    g_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    //g_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable multiple viewports

    ImGui_ImplSDL2_InitForOpenGL(_window, _ctx);
    ImGui_ImplOpenGL3_Init("#version 130");
}

MyWindow::~MyWindow() {

    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(_ctx);
    SDL_DestroyWindow(static_cast<SDL_Window*>(_window));
}

void MyWindow::swapBuffers() const {

    if (g_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(_window, _ctx);
    }

    SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window));
}

void MyWindow::setDefaultMaterial(const Material& material) {
    _defaultMaterial = material;
}

void MyWindow::handleFileDrop(const char* filePath, HierarchyPanel& hierarchyPanel) {
    std::string path(filePath);
    std::string extension = path.substr(path.find_last_of(".") + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // Convert to lowercase

    if (extension == "fbx") {
        std::cout << "File dropped: " << filePath << std::endl;
        auto gameObject = std::make_unique<GameObject>();
        if (gameObject->loadModel(filePath)) {
            gameObjects.push_back(std::move(gameObject));
        }
    } else if (extension == "png" || extension == "dds") {
        GameObject* selectedGameObject = hierarchyPanel.getSelectedGameObject();
        if (selectedGameObject) {
            Material& material = selectedGameObject->getMaterial();
            if (material.loadTexture(filePath)) {
                std::cout << "Texture loaded and set successfully: " << filePath << std::endl;
            } else {
                std::cout << "Failed to load texture: " << filePath << std::endl;
            }
        } else {
            Logger::GetInstance().Log("SELECT AN OBJECT TO ADD A TEXTURE", WARNING);
        }
    }
}

