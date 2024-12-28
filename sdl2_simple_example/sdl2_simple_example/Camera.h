#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL_events.h>
#include <vector>  // Añadir este include
#include <GL/glew.h>  // Añadir este include

class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void processKeyboard(SDL_Keycode key, float deltaTime);
    void update(float deltaTime);
    void resetFocus(const glm::vec3& targetPosition, const glm::vec3& meshSize);
    void processMousePan(float xoffset, float yoffset);
    void processMouseOrbit(float xoffset, float yoffset, const glm::vec3& targetPosition);

    void enableFPSMode(bool enable);
    bool isFPSModeEnabled() const;

    void updateFrustum();
    bool isBoxInFrustum(const glm::vec3& minBound, const glm::vec3& maxBound) const;

    // Nuevas funciones para obtener la posici�n y direcci�n del rayo
    glm::vec3 getPosition() const;
    glm::vec3 getRayDirection() const;

    // Clase Ray
    class Ray {
    public:
        Ray(const glm::vec3& origin, const glm::vec3& direction)
            : origin(origin), direction(glm::normalize(direction)) {
        }

        glm::vec3 origin;
        glm::vec3 direction;
    };

    // Generar rayo desde las coordenadas del rat�n
    Ray GenerateRay(int mouseX, int mouseY, int screenWidth, int screenHeight, float aspectRatio) const;

    void drawFrustumRays() const;  // Nueva función para dibujar los rayos
    float getFarPlane() const { return 100.0f; }  // Distancia del far plane
    float getNearPlane() const { return 0.1f; }   // Distancia del near plane

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    bool fpsMode;

    glm::vec4 frustumPlanes[6];

    void updateCameraVectors();

    // Convertir de coordenadas de pantalla a NDC
    glm::vec2 ScreenToNDC(int mouseX, int mouseY, int screenWidth, int screenHeight) const;

    void calculateFrustumCorners(std::vector<glm::vec3>& corners) const;
};

#endif
