#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp> 

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}

Camera::Camera()
    : position(glm::vec3(0.0f, 0.0f, 3.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)), worldUp(up), yaw(-90.0f), pitch(0.0f),
    movementSpeed(2.5f), mouseSensitivity(0.1f), zoom(45.0f), fpsMode(false) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(SDL_Keycode key, float deltaTime) {
    float velocity = movementSpeed * (fpsMode ? 2.0f : 1.0f) * deltaTime;
    if (key == SDLK_w) position += front * velocity;
    if (key == SDLK_s) position -= front * velocity;
    if (key == SDLK_a) position -= right * velocity;
    if (key == SDLK_d) position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
}

void Camera::processMouseOrbit(float xoffset, float yoffset, const glm::vec3& targetPosition) {
    float orbitSpeed = 1.0f;

    // Modificar el ángulo de la cámara según el movimiento del ratón
    yaw += xoffset * orbitSpeed;
    pitch += yoffset * orbitSpeed;

    // Limitar el ángulo de elevación (pitch)
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calcular la nueva dirección de la cámara basada en la orbita
    updateCameraVectors();

    // Posicionar la cámara en torno al objeto seleccionado
    position = targetPosition - front * glm::length(targetPosition - position);
}


void Camera::processMouseScroll(float yoffset) {
    // Aumentamos o disminuimos la distancia de la cámara, basándonos en el movimiento de la rueda del ratón
    float zoomSpeed = 0.1f;  // Controla qué tan rápido cambia la distancia
    position += front * yoffset * zoomSpeed;  // Ajustamos la posición de la cámara a lo largo de la dirección 'front'

    // Limitar la distancia de la cámara
    if (glm::length(position) < 1.0f) position = glm::normalize(position) * 1.0f;  // No dejar que la cámara se acerque demasiado
    if (glm::length(position) > 80.0f) position = glm::normalize(position) * 80.0f;  // No dejar que la cámara se aleje demasiado
}


void Camera::processMousePan(float xoffset, float yoffset) {
    float panSpeed = mouseSensitivity * 0.05f;
    position += right * -xoffset * panSpeed; // Mueve en X
    position -= up * yoffset * panSpeed;     // Mueve en Y
}

void Camera::update(float deltaTime) {}

void Camera::resetFocus(const glm::vec3& targetPosition, const glm::vec3& meshSize) {
    // Calcular la distancia de la cámara en función del tamaño de la malla
    float distance = glm::length(meshSize) * 0.2f;  // Multiplicamos por un factor para dar espacio

    // Ajustar la posición de la cámara
    position = targetPosition - front * distance;

    // Actualizamos los vectores de la cámara
    updateCameraVectors();
}

void Camera::enableFPSMode(bool enable) {
    fpsMode = enable;
}

bool Camera::isFPSModeEnabled() const {
    return fpsMode;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
