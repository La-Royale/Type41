#include "Logger.h"

void Logger::Log(const std::string& message, LogType type) {
    if (consolePanel) {
        consolePanel->Log(message.c_str(), type);
    }
}