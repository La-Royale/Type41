#include "MainMenu.h"
#include "SDL2/SDL.h"
#include "imgui.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>


//MainMenu::MainMenu() {
//}

//MainMenu::~MainMenu() {
//}

void MainMenu::Render(bool& showConsole, bool& showConfig, bool& showHierarchy, bool& showInspector) {


    //if (ImGui::BeginMainMenuBar()) {
    //    if (ImGui::BeginMenu("Menu")) {
    //        if (ImGui::MenuItem("Quit")) {
    //            SDL_Event quit_event;
    //            quit_event.type = SDL_QUIT;
    //            SDL_PushEvent(&quit_event);
    //        }
    //        ImGui::EndMenu();
    //        
    //        if (ImGui::BeginMenu("Windows")) {
    //        ImGui::MenuItem("Console", NULL, &showConsole);
    //        ImGui::MenuItem("Config", NULL, &showConfig);
    //        ImGui::MenuItem("Hierarchy", NULL, &showHierarchy);
    //        ImGui::MenuItem("Inspector", NULL, &showInspector);
    //        ImGui::EndMenu();
    //        }
    //        ImGui::EndMainMenuBar();
    //    }
    //}


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("Quit")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }

            if (ImGui::BeginMenu("Windows")) {
                ImGui::MenuItem("Console", NULL, &showConsole);
                ImGui::MenuItem("Config", NULL, &showConfig);
                ImGui::MenuItem("Hierarchy", NULL, &showHierarchy);
                ImGui::MenuItem("Inspector", NULL, &showInspector);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
