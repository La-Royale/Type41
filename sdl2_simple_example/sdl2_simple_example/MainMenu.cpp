#include "MainMenu.h"
#include "SDL2/SDL.h"
#include "imgui.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include "ModelLoader.h"

ModelLoader modelLoader;
void MainMenu::Render(bool& showConsole, bool& showConfig, bool& showHierarchy, bool& showInspector) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("Primitive")) {
                if (ImGui::MenuItem("Cube")) {
                    modelLoader.createCube(); 
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
                ImGui::MenuItem("Config", NULL, &showConfig);
                ImGui::MenuItem("Hierarchy", NULL, &showHierarchy);
                ImGui::MenuItem("Inspector", NULL, &showInspector);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
