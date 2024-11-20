#pragma once
#include <string>
#include "Material.h"
#include "HierarchyPanel.h"  // Incluimos el header de HierarchyPanel

struct SDL_Window;

class MyWindow {

    SDL_Window* _window = nullptr;
    void* _ctx = nullptr;

    int _width = 0;
    int _height = 0;

public:
    int width() const { return _width; }
    int height() const { return _height; }
    double aspectRatio() const { return static_cast<double>(_width) / _height; }

    MyWindow(const std::string& title, int w, int h);
    ~MyWindow();

    // Métodos para manejar el buffer de la ventana
    void swapBuffers() const;

    // Método para manejar el archivo que se arrastra
    void handleFileDrop(const char* filePath, HierarchyPanel& hierarchyPanel);

    // Método para establecer el material por defecto
    void setDefaultMaterial(const Material& material);

    // Nuevo método para obtener la ventana SDL
    SDL_Window* getWindow() const { return _window; }

private:
    Material _defaultMaterial;
};
