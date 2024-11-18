#include <GL/glew.h>
#include <iostream>
#include <GL/gl.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <SDL2/SDL_events.h>
#include <memory>
#include <vector>

#include "MyWindow.h"
#include "imgui_impl_sdl2.h"
#include "WindowEditor.h"
#include "ModelLoader.h"
#include "Material.h"
#include "Camera.h" 

#include "GameObject.h"
#include "HierarchyPanel.h"
#include "ConsolePanel.h"
#include "ScenePanel.h"

using namespace std;
using hrclock = chrono::high_resolution_clock;
using ivec2 = glm::ivec2;

static const ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
    glewInit();
    if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.5, 1.0);

    glMatrixMode(GL_PROJECTION);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WINDOW_SIZE.x) / WINDOW_SIZE.y, 0.1f, 100.0f);
    glLoadMatrixf(&projection[0][0]);
    glMatrixMode(GL_MODELVIEW);
}

std::vector<std::unique_ptr<GameObject>> gameObjects;

Material defaultMaterial;

static bool processEvents(MyWindow& window, Camera& camera, HierarchyPanel& hierarchyPanel, float deltaTime) {
    SDL_Event event;
    bool isAltPressed = false;  // Esta variable controlará el estado de la tecla Alt
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_f) {
                // Centrar la cámara en el objeto seleccionado
                GameObject* selectedGameObject = hierarchyPanel.getSelectedGameObject();
                if (selectedGameObject) {
                    glm::vec3 meshSize = selectedGameObject->getMeshSize();
                    camera.resetFocus(selectedGameObject->getPosition(), meshSize);
                }
            }
            else if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                camera.enableFPSMode(true);
            }
            // Detectamos cuando se presiona la tecla Alt
            else if (event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_RALT) {
                isAltPressed = true;  // Activamos el estado de Alt
            }
            // También procesamos el movimiento WASD aquí, independientemente de Alt
            else {
                camera.processKeyboard(event.key.keysym.sym, deltaTime);
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                camera.enableFPSMode(false);
            }
            // Detectamos cuando se suelta la tecla Alt
            else if (event.key.keysym.sym == SDLK_LALT || event.key.keysym.sym == SDLK_RALT) {
                isAltPressed = false;  // Desactivamos el estado de Alt
            }
            break;
        case SDL_MOUSEMOTION:
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                camera.processMouseMovement(event.motion.xrel, -event.motion.yrel);
            }
            else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
                camera.processMousePan(event.motion.xrel, -event.motion.yrel); // Pan con el botón central
            }
            else if (isAltPressed && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                // Orbitación con el botón izquierdo y ALT
                GameObject* selectedGameObject = hierarchyPanel.getSelectedGameObject();
                if (selectedGameObject) {
                    camera.processMouseOrbit(event.motion.xrel, -event.motion.yrel, selectedGameObject->getPosition());
                }
            }
            ImGui_ImplSDL2_ProcessEvent(&event);
            break;
        case SDL_MOUSEWHEEL:
            camera.processMouseScroll(event.wheel.y);
            break;
        case SDL_DROPFILE:
            std::cout << "File drop event detected" << std::endl;
            window.handleFileDrop(event.drop.file, hierarchyPanel);
            SDL_free(event.drop.file);
            break;
        default:
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }
    return true;
}




int main(int argc, char** argv) {

    // Crear la ventana
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
    init_openGL();
    // Crear el panel de configuración, pasándole la referencia de la ventana
    ConfigPanel configPanel(&window);
    ScenePanel scenePanel;

    // Establecer color por defecto
    defaultMaterial.setDefaultColor(glm::vec3(0.8f, 0.8f, 0.8f));
    window.setDefaultMaterial(defaultMaterial);

    // Crear el panel de jerarquía (debe ser una referencia)
    HierarchyPanel hierarchyPanel;

    ScenePanel scenePanel;
    //scenePanel.SetSceneObjects(gameObjects);

    // Crear algunos objetos de juego y cargarlos
    auto gameObject1 = std::make_unique<GameObject>();
    gameObject1->loadModel("Assets/Models/BakerHouse.fbx");

    auto gameObject2 = std::make_unique<GameObject>();
    gameObject2->loadModel("Assets/Models/PremiumHouse.fbx");
    gameObject2->setPosition(glm::vec3(1.0f, 0.2f, 0.0f));
    gameObject2->setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));


    /*auto gameObject3 = std::make_unique<GameObject>();
    gameObject3->loadModel("Assets/Models/AngryDragon.fbx");
    gameObject3->setPosition(glm::vec3(0.0f, 0.8f, 0.0f));
    gameObject3->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    gameObject3->setScale(glm::vec3(0.1f, 0.1f, 0.1f));*/

    // Establecer materiales
    Material material;
    material.loadTexture("Assets/Textures/Baker_house.png");
    gameObject1->setMaterial(material);

    material.loadTexture("Assets/Textures/Premium_house.png");
    gameObject2->setMaterial(material);    
    
    //material.loadTexture("Assets/Textures/Angry_dragon.png");
    //gameObject3->setMaterial(material);

    // Agregar objetos de juego a la lista
    gameObjects.push_back(std::move(gameObject1));
    gameObjects.push_back(std::move(gameObject2));
    //gameObjects.push_back(std::move(gameObject3));

    // Crear el editor de la ventana y pasarle la referencia de hierarchyPanel y la ventana
    WindowEditor editor(hierarchyPanel, &window);  // Asegúrate de que se pase la referencia correcta

    Camera camera;
    float deltaTime = 0.0f;
    auto lastFrame = hrclock::now();

    // Bucle principal de la aplicación
    // Bucle principal de la aplicación
    while (processEvents(window, camera, hierarchyPanel, deltaTime)) {
        const auto t0 = hrclock::now();
        deltaTime = chrono::duration<float>(t0 - lastFrame).count();
        lastFrame = t0;

        // Limpia el buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Actualiza la proyección de la cámara
        glm::mat4 projection = camera.getProjectionMatrix(float(WINDOW_SIZE.x) / WINDOW_SIZE.y);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&projection[0][0]);
        glMatrixMode(GL_MODELVIEW);

        // Matriz de vista de la cámara
        glm::mat4 view = camera.getViewMatrix();
        glLoadMatrixf(&view[0][0]);

        // Llama al renderizado de la escena dentro del ScenePanel
        //scenePanel.Render();

        // Renderizar el editor de la ventana
        editor.Render(gameObjects);

        // Cambia los buffers
        window.swapBuffers();

        // Control de FPS
        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }


    return 0;
}