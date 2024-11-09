// ConsolePanel.cpp
#include "ConsolePanel.h"
#include <iostream>

ConsolePanel::ConsolePanel() {
    // Reserva de espacio en el vector para 1000 mensajes
    messages.reserve(1000);
}

ConsolePanel::~ConsolePanel() {}

void ConsolePanel::Render() {
    ImGui::Begin("Console");

    // Mostrar los mensajes en la consola con colores según el tipo de mensaje
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
    std::cout << "ConsolePanel: Agregando mensaje: " << message << std::endl;  // Debug
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

    messages.push_back(formattedMessage);  // Agregar al vector

    // Limitar el número de mensajes a 1000
    if (messages.size() > 1000) {
        messages.erase(messages.begin());
    }
}
