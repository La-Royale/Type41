#include "ScenePanel.h"
<<<<<<< Updated upstream
#include <iostream>
#include <imgui.h>
#include <GL/glew.h>
#include <GL/gl.h>

ScenePanel::ScenePanel()
    : fbo(0), texture(0), rbo(0), panelWidth(800), panelHeight(600) {
    Init();
}

ScenePanel::~ScenePanel() {
    CleanupFramebuffer();
}

void ScenePanel::Init() {
    // Crear el framebuffer inicial
    CreateFramebuffer(panelWidth, panelHeight);
}

void ScenePanel::SetGameObjects(const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    this->gameObjects = gameObjects;
}

void ScenePanel::SetCamera(const Camera& camera) {
    this->camera = camera;
}

void ScenePanel::Render() {
    // Vincula el FBO para renderizar la escena
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, panelWidth, panelHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Llama a RenderScene con gameObjects y camera
    RenderScene(gameObjects, camera);

    // Desvincula el FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Muestra la textura en el panel de ImGui
    RenderImGui();
}

void ScenePanel::RenderScene(const std::vector<std::unique_ptr<GameObject>>& gameObjects, const Camera& camera) {
    // Establece la proyección y la vista de la cámara
    glm::mat4 projection = camera.getProjectionMatrix(float(panelWidth) / panelHeight);
    glm::mat4 view = camera.getViewMatrix();

    // Configura las matrices de OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&projection[0][0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&view[0][0]);

    // Itera sobre los objetos del juego y dibuja cada uno
    for (const auto& gameObject : gameObjects) {
        gameObject->draw();  // Suponiendo que los GameObjects tienen un método `draw()`
    }
}


void ScenePanel::Resize(int width, int height) {
    if (width != panelWidth || height != panelHeight) {
        panelWidth = width;
        panelHeight = height;

        // Actualizar el FBO al nuevo tamaño
        CleanupFramebuffer();
        CreateFramebuffer(panelWidth, panelHeight);
    }
}

void ScenePanel::CreateFramebuffer(int width, int height) {
    // Genera el FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Genera la textura de color
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Crear el renderbuffer para el buffer de profundidad
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer no está completo" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Desvincula el framebuffer
}

void ScenePanel::CleanupFramebuffer() {
    // Liberar los recursos de OpenGL
    if (fbo) {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
    if (texture) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
    if (rbo) {
        glDeleteRenderbuffers(1, &rbo);
        rbo = 0;
    }
}

void ScenePanel::RenderImGui() {
    ImGui::Begin("Scene");

    // Obtener el tamaño disponible en el panel de ImGui y ajustar el framebuffer si cambia
    ImVec2 panelSize = ImGui::GetContentRegionAvail();
    if (panelSize.x != panelWidth || panelSize.y != panelHeight) {
        Resize(panelSize.x, panelSize.y);
    }

    // Mostrar la textura del framebuffer en el panel de ImGui
    ImGui::Image((void*)(intptr_t)texture, ImVec2(panelWidth, panelHeight), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
=======
#include <vector>
#include <string>

// Constructor y destructor sin cambios
ScenePanel::ScenePanel() {}
ScenePanel::~ScenePanel() {}

// Método para renderizar el panel de escena
void ScenePanel::Render() {
    ImGui::Begin("Scene");

    // Ejemplo: Iterar sobre una lista de objetos en la escena
    if (sceneObjects) {
        for (size_t i = 0; i < sceneObjects->size(); ++i) {
            const std::string& objectName = (*sceneObjects)[i];
            ImGui::Text("%zu: %s", i + 1, objectName.c_str());
        }
    }
    else {
        ImGui::Text("No hay objetos en la escena.");
    }

    ImGui::End();
}

// Método para configurar la lista de objetos
void ScenePanel::SetSceneObjects(const std::vector<std::string>* objects) {
    sceneObjects = objects;
>>>>>>> Stashed changes
}