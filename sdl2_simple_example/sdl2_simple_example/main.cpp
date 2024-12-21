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
#include <unordered_map>

#include "Framebuffer.h"
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

GLuint sceneFramebuffer, sceneTexture, depthRenderbuffer;
Framebuffer framebuffer;
WindowEditor* editor; // Declare editor as a global pointer

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
            // Tambi��n procesamos el movimiento WASD aquí, independientemente de Alt
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
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int newWidth = event.window.data1;
                int newHeight = event.window.data2;
                framebuffer.Resize(newWidth, newHeight);
                editor->SetFramebuffer(framebuffer.GetTexture()); // Update framebuffer texture in editor
            }
            break;
        default:
            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }
    return true;
}

void initFramebuffer(int width, int height) {
    glGenFramebuffers(1, &sceneFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer);

    // Crear una textura para el color del framebuffer
    glGenTextures(1, &sceneTexture);
    glBindTexture(GL_TEXTURE_2D, sceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Asignar la textura al framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

    // Crear un renderbuffer para el buffer de profundidad
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    // Verificar si el framebuffer está completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer no está completo" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Desvincular framebuffer
}

void resizeFramebuffer(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, sceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::unordered_map<std::string, glm::vec3> initialPositions;
std::unordered_map<std::string, glm::vec3> initialRotations;
std::unordered_map<std::string, glm::vec3> initialScales;


int main(int argc, char** argv) {

    // Crear la ventana
    MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);

    // Crear el panel de configuración, pasándole la referencia de la ventana
    ConfigPanel configPanel(&window);

    // Inicializar OpenGL
    init_openGL();
    framebuffer.Init(WINDOW_SIZE.x, WINDOW_SIZE.y);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error (Framebuffer Init): " << err << std::endl;
    }

    // Establecer color por defecto
    defaultMaterial.setDefaultColor(glm::vec3(0.8f, 0.8f, 0.8f));
    window.setDefaultMaterial(defaultMaterial);

    // Crear el panel de jerarquía (debe ser una referencia)
    HierarchyPanel hierarchyPanel;

    // Crear algunos objetos de juego y cargarlos
    auto gameObject1 = std::make_unique<GameObject>();
    gameObject1->loadModel("Assets/Models/Street environment_V01.fbx");

    // Establecer materiales
    Material material;
    material.loadTexture("Assets/Textures/Building_V02_C.png");
    gameObject1->setMaterial(material);

    // Agregar objetos de juego a la lista
    gameObjects.push_back(std::move(gameObject1));  

    // Crear el editor de la ventana y pasarle la referencia de hierarchyPanel y la ventana
    editor = new WindowEditor(hierarchyPanel, &window);  // Asegúrate de que se pase la referencia correcta
    editor->SetFramebuffer(framebuffer.GetTexture());

    // Crear el panel de escena
    ScenePanel scenePanel;
    scenePanel.SetFramebufferTexture(framebuffer.GetTexture());

    // Guardar posición inicial de los objetos
    for (const auto& gameObject : gameObjects) {
        initialPositions[gameObject->getName()] = gameObject->getPosition();
        initialRotations[gameObject->getName()] = gameObject->getRotation();
        initialScales[gameObject->getName()] = gameObject->getScale();
    }

    Camera camera;
    float deltaTime = 0.0f;
    auto lastFrame = hrclock::now();

    std::unordered_map<std::string, bool> objectVisibility;

    bool resetObjects = false;
    bool saveObjects = false;

    // Bucle principal de la aplicación
    while (processEvents(window, camera, hierarchyPanel, deltaTime)) {
        const auto t0 = hrclock::now();
        deltaTime = chrono::duration<float>(t0 - lastFrame).count();
        lastFrame = t0;

        // **1. Renderizar al framebuffer**
        framebuffer.Bind();  // Vincular framebuffer
        glViewport(0, 0, framebuffer.width(), framebuffer.height()); // Ajustar el viewport al tamaño del framebuffer
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);        // Ajustar el viewport
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Limpiar buffers

        // Actualizar la proyección y vista de la cámara
        glm::mat4 projection = camera.getProjectionMatrix(float(framebuffer.width()) / framebuffer.height());
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&projection[0][0]);
        glMatrixMode(GL_MODELVIEW);

        glm::mat4 view = camera.getViewMatrix();
        glLoadMatrixf(&view[0][0]);

        camera.updateFrustum();

        // Dibujar objetos de la escena
        for (auto& gameObject : gameObjects) {
            bool isVisible = camera.isBoxInFrustum(gameObject->getGlobalMinBound(), gameObject->getGlobalMaxBound());
            const std::string& name = gameObject->getName();

            if (isVisible) {
                gameObject->draw();
                if (objectVisibility[name] == false) {
                    //std::cout << "Object " << name << " is now visible." << std::endl;
                    objectVisibility[name] = true;
                }
            } else {
                if (objectVisibility[name] == true) {
                    //std::cout << "Object " << name << " is now hidden." << std::endl;
                    objectVisibility[name] = false;
                }
            }
        }

        if (editor->simulationPanel->GetState() == SimulationState::RUNNING) {
            // Save current state of the scene
            if(saveObjects){
                for (const auto& gameObject : gameObjects) {
                    initialPositions[gameObject->getName()] = gameObject->getPosition();
                    initialRotations[gameObject->getName()] = gameObject->getRotation();
                    initialScales[gameObject->getName()] = gameObject->getScale();
                }
                saveObjects = false;
            }

            // Update game objects (Por ahora no hace nada)
            for (auto& gameObject : gameObjects) {
                gameObject->update(deltaTime);
            }
            resetObjects = true;
        } else if (editor->simulationPanel->GetState() == SimulationState::STOPPED && resetObjects) {
            // Reset game objects to initial positions / rotations / scales
            for (auto& gameObject : gameObjects) {
                gameObject->setPosition(initialPositions[gameObject->getName()]);
                gameObject->setRotation(initialRotations[gameObject->getName()]);
                gameObject->setScale(initialScales[gameObject->getName()]);
            }
            resetObjects = false;
            saveObjects = true;
        }

        framebuffer.Unbind(); // Desvincular framebuffer

        // Renderizar la interfaz de usuario (ImGui)
        editor->Render(gameObjects);  // Aquí se incluye el panel con la textura del framebuffer

        // Intercambiar buffers
        window.swapBuffers();

        const auto t1 = hrclock::now();
        const auto dt = t1 - t0;
        if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
    }

    delete editor; // Clean up the editor

    return 0;
}