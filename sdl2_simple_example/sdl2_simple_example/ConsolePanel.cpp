// ConsolePanel.cpp
#include "ConsolePanel.h"

ConsolePanel::ConsolePanel() {
    // Reserva de espacio en el vector para 1000 mensajes
    messages.reserve(1000);
}

ConsolePanel::~ConsolePanel() {}

void ConsolePanel::Render() {
    ImGui::Begin("Console");

    // Mostrar los mensajes en la consola
    for (const auto& message : messages) {
        ImGui::TextUnformatted(message.c_str());
    }

    ImGui::End();
}

void ConsolePanel::Log(const char* message, LogType type) {
    std::string formattedMessage;
    switch (type) {
        case INFO:
            formattedMessage = "[INFO] " + std::string(message);
            break;
        case WARNING:
            formattedMessage = "[WARNING] " + std::string(message);
            break;
        case ERROR:
            formattedMessage = "[ERROR] " + std::string(message);
            break;
    }

    // Agregar el mensaje al vector de logs
    messages.push_back(formattedMessage);

    // Limitar el número de mensajes a 1000
    if (messages.size() > 1000) {
        messages.erase(messages.begin());  // Eliminar el mensaje más antiguo
    }
}
