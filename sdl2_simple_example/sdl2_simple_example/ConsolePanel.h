#ifndef CONSOLE_PANEL_H
#define CONSOLE_PANEL_H

#include <vector>
#include <string>
#include "imgui.h"

enum LogType {
    INFO,
    WARNING,
    INTRO

};

class ConsolePanel {
public:
    ConsolePanel();
    ~ConsolePanel();

    void Render();


    void Log(const char* message, LogType type = INFO);

private:
    std::vector<std::string> messages;
};

#endif