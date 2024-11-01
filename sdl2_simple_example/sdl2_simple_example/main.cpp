#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include <iostream>
#include <GL/gl.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include <memory>
#include "MyWindow.h"
#include "imgui_impl_sdl2.h"
#include "WindowEditor.h"
#include "ModelLoader.h"
#include "Material.h"
#include "GameObject.h"

using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1600, 900);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void display_func(ModelLoader& modelLoader) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    modelLoader.drawModel();
    glRotatef(0.8f, 1.0f, 1.0f, 0.0f);
}

static bool processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;
        default:
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }
    return true;
}

int main(int argc, char** argv) {
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
    init_openGL();
    
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    auto gameObject1 = std::make_unique<GameObject>();
    gameObject1->loadModel("BakerHouse.fbx");

    // Crear y aplicar el material con la textura
    Material material;
    material.loadTexture("Baker_house.png");

    gameObject1->setMaterial(material);

    gameObjects.push_back(std::move(gameObject1));

    // auto gameObject2 = std::make_unique<GameObject>();
    // gameObject2->loadModel("masterchief.fbx");
    // gameObjects.push_back(std::move(gameObject2));

    // Añade más GameObjects según sea necesario
    // auto gameObjectN = std::make_unique<GameObject>();
    // gameObjectN->loadModel("otro_modelo.fbx");
    // gameObjects.push_back(std::move(gameObjectN));

    WindowEditor editor;

    while (processEvents()) {
        const auto t0 = hrclock::now();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& gameObject : gameObjects) {
            gameObject->draw();
        }
        glRotatef(0.8f, 1.0f, 1.0f, 0.0f);

        editor.Render(); // Llama a Render en cada fotograma

        window.swapBuffers();
        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    return 0;
}