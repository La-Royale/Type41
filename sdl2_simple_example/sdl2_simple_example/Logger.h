#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "ConsolePanel.h"

class Logger {
public:
    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }


    void Log(const std::string& message, LogType type) {

        if (consolePanel) {
            //std::cout << "ConsolePanel est� asignado. Enviando log..." << std::endl;  // Debug
            consolePanel->Log(message.c_str(), type);  
        }
        else {
            //std::cout << "ConsolePanel no est� asignado" << std::endl;  // Debug
        }
    }



    void SetConsolePanel(ConsolePanel* panel) {
        if (panel) { 
            consolePanel = panel;
        }
        else {
            std::cerr << "Error: ConsolePanel no puede ser nullptr" << std::endl;
        }
    }

private:
    ConsolePanel* consolePanel = nullptr; 


    Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
