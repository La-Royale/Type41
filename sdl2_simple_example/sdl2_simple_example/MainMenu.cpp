#include "MainMenu.h"
#include "SDL2/SDL.h"
#include "imgui.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include "GameObject.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>

extern std::vector<std::unique_ptr<GameObject>> gameObjects;
extern Material defaultMaterial;

void MainMenu::Render(bool& showConsole, bool& showConfig, bool& showHierarchy, bool& showInspector, bool& showScene) {
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("Primitive")) {
                if (ImGui::MenuItem("Cube")) {
                    auto gameObject = std::make_unique<GameObject>();
                    if (gameObject->loadModel("Assets/Primitives/Cube.fbx")) {
                        gameObject->setMaterial(defaultMaterial);
                        gameObjects.push_back(std::move(gameObject));
                    }
                }
                if (ImGui::MenuItem("Sphere")) {
                    auto gameObject = std::make_unique<GameObject>();
                    if (gameObject->loadModel("Assets/Primitives/Sphere.fbx")) {
                        gameObject->setMaterial(defaultMaterial);
                        gameObjects.push_back(std::move(gameObject));
                    }
                }
                if (ImGui::MenuItem("Plane")) {
                    auto gameObject = std::make_unique<GameObject>();
                    if (gameObject->loadModel("Assets/Primitives/Plane.fbx")) {
                        gameObject->setMaterial(defaultMaterial);
                        gameObjects.push_back(std::move(gameObject));
                    }
                }
                if (ImGui::MenuItem("Cylinder")) {
                    auto gameObject = std::make_unique<GameObject>();
                    if (gameObject->loadModel("Assets/Primitives/Cylinder.fbx")) {
                        gameObject->setMaterial(defaultMaterial);
                        gameObjects.push_back(std::move(gameObject));
                    }
                }
                if (ImGui::MenuItem("Cone")) {
                    auto gameObject = std::make_unique<GameObject>();
                    if (gameObject->loadModel("Assets/Primitives/Cone.fbx")) {
                        gameObject->setMaterial(defaultMaterial);
                        gameObjects.push_back(std::move(gameObject));
                    }
                }
                if (ImGui::MenuItem("Torus")) {
                    auto gameObject = std::make_unique<GameObject>();
                    if (gameObject->loadModel("Assets/Primitives/Torus.fbx")) {
                        gameObject->setMaterial(defaultMaterial);
                        gameObjects.push_back(std::move(gameObject));
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("About")) {
                SDL_OpenURL("https://github.com/La-Royale/Type41");
            }

            if (ImGui::BeginMenu("Windows")) {
                ImGui::MenuItem("Console", NULL, &showConsole);
                ImGui::MenuItem("Configuration", NULL, &showConfig);
                ImGui::MenuItem("Hierarchy", NULL, &showHierarchy);
                ImGui::MenuItem("Inspector", NULL, &showInspector);
                ImGui::MenuItem("Scene", NULL, &showScene);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}