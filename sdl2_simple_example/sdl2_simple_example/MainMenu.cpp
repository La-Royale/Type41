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



void MainMenu::Render(bool& showConsole, bool& showConfig, bool& showHierarchy, bool& showInspector) {
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("Primitive")) {
                if (ImGui::MenuItem("Cube")) {

                }
                if (ImGui::MenuItem("Sphere")) {

                }
                if (ImGui::MenuItem("Plane")) {

                }
                if (ImGui::MenuItem("Cylinder")) {

                }
                if (ImGui::MenuItem("Cone")) {

                }
                if (ImGui::MenuItem("Torus")) {

                }
                if (ImGui::MenuItem("Pyramid")) {

                }
                if (ImGui::MenuItem("Capsule")) {

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
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}