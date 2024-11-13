#ifndef SCENEPANEL_H
#define SCENEPANEL_H

#include <vector>
#include <memory>
#include "GameObject.h"
#include "Camera.h"

class ScenePanel {
public:
    ScenePanel();
    ~ScenePanel();

    // Inicializa el panel
    void Init();

    void SetGameObjects(const std::vector<std::unique_ptr<GameObject>>& gameObjects);
    void SetCamera(const Camera& camera);

    // Renderiza la escena en el framebuffer y la muestra en ImGui
    void Render();

    // Cambia el tamaño del framebuffer al tamaño especificado
    void Resize(int width, int height);

    // Configura el contenido de la escena
    void SetSceneContent(const std::vector<std::unique_ptr<GameObject>>& gameObjects, const Camera& camera);

private:
    // Renderiza la escena al framebuffer
    void RenderScene(const std::vector<std::unique_ptr<GameObject>>& gameObjects, const Camera& camera);

    // Muestra la textura de la escena en el panel de ImGui
    void RenderImGui();

    // Crear el framebuffer para renderizar
    void CreateFramebuffer(int width, int height);

    // Limpiar el framebuffer liberando recursos
    void CleanupFramebuffer();

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    Camera camera;

    unsigned int fbo;           // Framebuffer Object
    unsigned int texture;       // Textura para renderizar la escena
    unsigned int rbo;           // Renderbuffer Object para profundidad/stencil

    int panelWidth;             // Ancho del panel de escena
    int panelHeight;            // Alto del panel de escena
};

#endif // SCENEPANEL_H
