#include "ConsolePanel.h"
#include <iostream>

ConsolePanel::ConsolePanel() {
    messages.reserve(1000);
}

ConsolePanel::~ConsolePanel() {}

void ConsolePanel::Render() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

    //ImGui::SetNextWindowPos(ImVec2(176, 545));
    //ImGui::SetNextWindowSize(ImVec2(770, 174));

    ImGui::Begin("Console"/*, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar*/);

    for (const auto& message : messages) {
        if (message.find("[INFO]") != std::string::npos) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.8f, 1.0f, 1.0f));  // Azul para INFO
        }
        else if (message.find("[WARNING]") != std::string::npos) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));  // Amarillo para WARNING
        }
        else if (message.find("[INTRO]") != std::string::npos) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));  // Rojo para ERROR
        }

        ImGui::TextUnformatted(message.c_str());
        ImGui::PopStyleColor();  // Restaurar el color
    }

    ImGui::End();
}


void ConsolePanel::Log(const char* message, LogType type) {
    //std::cout << "ConsolePanel: Agregando mensaje: " << message << std::endl;  // Debug
    std::string formattedMessage;
    switch (type) {
        case INFO:
            formattedMessage = "[INFO] " + std::string(message);
            break;
        case WARNING:
            formattedMessage = "[WARNING] " + std::string(message);
            break;
        case INTRO:
            formattedMessage = "[INTRO] " + std::string(message);
            break;
    }

    messages.push_back(formattedMessage);

    if (messages.size() > 1000) {
        messages.erase(messages.begin());
    }
}
