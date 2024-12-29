#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>  // Añadir este include si no está ya incluido a través de Camera.h

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

    yaw += xoffset * orbitSpeed;
    pitch += yoffset * orbitSpeed;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();

    position = targetPosition - front * glm::length(targetPosition - position);
}

void Camera::processMouseScroll(float yoffset) {
    float zoomSpeed = 0.1f;
    position += front * yoffset * zoomSpeed;

    if (glm::length(position) < 1.0f) position = glm::normalize(position) * 1.0f;
    if (glm::length(position) > 80.0f) position = glm::normalize(position) * 80.0f;
}

void Camera::processMousePan(float xoffset, float yoffset) {
    float panSpeed = mouseSensitivity * 0.05f;
    position += right * -xoffset * panSpeed;
    position -= up * yoffset * panSpeed;
}

void Camera::update(float deltaTime) {}

void Camera::resetFocus(const glm::vec3& targetPosition, const glm::vec3& meshSize) {
    float distance = glm::length(meshSize) * 0.2f;

    position = targetPosition - front * distance;

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

void Camera::updateFrustum() {
    glm::mat4 proj = getProjectionMatrix(1920.0f/1080.0f); // Usa tu aspect ratio real
    glm::mat4 view = getViewMatrix();
    glm::mat4 vp = proj * view;

    // Extraer planos del frustum de la matriz de vista-proyección
    // Left plane
    frustumPlanes[0].x = vp[0][3] + vp[0][0];
    frustumPlanes[0].y = vp[1][3] + vp[1][0];
    frustumPlanes[0].z = vp[2][3] + vp[2][0];
    frustumPlanes[0].w = vp[3][3] + vp[3][0];

    // Right plane
    frustumPlanes[1].x = vp[0][3] - vp[0][0];
    frustumPlanes[1].y = vp[1][3] - vp[1][0];
    frustumPlanes[1].z = vp[2][3] - vp[2][0];
    frustumPlanes[1].w = vp[3][3] - vp[3][0];

    // Bottom plane
    frustumPlanes[2].x = vp[0][3] + vp[0][1];
    frustumPlanes[2].y = vp[1][3] + vp[1][1];
    frustumPlanes[2].z = vp[2][3] + vp[2][1];
    frustumPlanes[2].w = vp[3][3] + vp[3][1];

    // Top plane
    frustumPlanes[3].x = vp[0][3] - vp[0][1];
    frustumPlanes[3].y = vp[1][3] - vp[1][1];
    frustumPlanes[3].z = vp[2][3] - vp[2][1];
    frustumPlanes[3].w = vp[3][3] - vp[3][1];

    // Near plane
    frustumPlanes[4].x = vp[0][2];
    frustumPlanes[4].y = vp[1][2];
    frustumPlanes[4].z = vp[2][2];
    frustumPlanes[4].w = vp[3][2];

    // Far plane
    frustumPlanes[5].x = vp[0][3] - vp[0][2];
    frustumPlanes[5].y = vp[1][3] - vp[1][2];
    frustumPlanes[5].z = vp[2][3] - vp[2][2];
    frustumPlanes[5].w = vp[3][3] - vp[3][2];

    // Normalizar los planos
    for (int i = 0; i < 6; ++i) {
        float length = sqrtf(frustumPlanes[i].x * frustumPlanes[i].x +
                           frustumPlanes[i].y * frustumPlanes[i].y +
                           frustumPlanes[i].z * frustumPlanes[i].z);
        frustumPlanes[i] /= length;
    }
}

bool Camera::isBoxInFrustum(const glm::vec3& minBound, const glm::vec3& maxBound) const {
    // Para cada plano del frustum
    for (int i = 0; i < 6; i++) {
        glm::vec3 p(minBound);
        glm::vec3 n(maxBound);

        // Encontrar el punto p-vertex
        if (frustumPlanes[i].x >= 0) {
            p.x = maxBound.x;
            n.x = minBound.x;
        }
        if (frustumPlanes[i].y >= 0) {
            p.y = maxBound.y;
            n.y = minBound.y;
        }
        if (frustumPlanes[i].z >= 0) {
            p.z = maxBound.z;
            n.z = minBound.z;
        }

        // Si el punto p-vertex está fuera, el objeto está completamente fuera
        if (frustumPlanes[i].x * p.x + 
            frustumPlanes[i].y * p.y + 
            frustumPlanes[i].z * p.z + 
            frustumPlanes[i].w <= 0) {
            return false;
        }
    }
    
    return true;
}

glm::vec2 Camera::ScreenToNDC(int mouseX, int mouseY, int screenWidth, int screenHeight) const {
    float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / screenHeight;
    return glm::vec2(ndcX, ndcY);
}

//ESTO NO ESTA FUNCIONANDO
#include <iostream>  // Para usar std::cout

Camera::Ray Camera::GenerateRay(int mouseX, int mouseY, int screenWidth, int screenHeight, float aspectRatio) const {
    // Convertir las coordenadas del ratón a Normalized Device Coordinates (NDC)
    glm::vec2 ndc = ScreenToNDC(mouseX, mouseY, screenWidth, screenHeight);

    // Crear el rayo en el espacio de clip
    glm::vec4 rayClip(ndc.x, ndc.y, -1.0f, 1.0f);

    // Convertir el rayo a espacio de la cámara (Eye space)
    glm::vec4 rayEye = glm::inverse(getProjectionMatrix(aspectRatio)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Z = -1.0f y W = 0.0f para obtener un rayo

    // Convertir el rayo a espacio mundial
    glm::vec3 rayWorld = glm::vec3(glm::inverse(getViewMatrix()) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    // Log de la dirección del rayo
    std::cout << "Ray Origin: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
    std::cout << "Ray Direction: (" << rayWorld.x << ", " << rayWorld.y << ", " << rayWorld.z << ")\n";

    // Calcular el punto final del rayo (distancia arbitraria de 100.0f)
    glm::vec3 rayEnd = position + rayWorld * 100.0f; // Factor 100.0f para longitud del rayo
    std::cout << "Ray End Point: (" << rayEnd.x << ", " << rayEnd.y << ", " << rayEnd.z << ")\n";

    // Ahora, dibujamos el rayo directamente dentro de esta función
    glBegin(GL_LINES); // Inicia el dibujo de una línea
    glColor3f(1.0f, 0.0f, 0.0f); // Color rojo para el rayo
    glVertex3f(position.x, position.y, position.z); // Origen del rayo
    glVertex3f(rayEnd.x, rayEnd.y, rayEnd.z); // Punto final del rayo
    glEnd(); // Termina el dibujo

    return Ray(position, rayWorld); // Retorna el objeto Ray con el origen y dirección calculados
}



glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getRayDirection() const {
    return front;
}

void Camera::calculateFrustumCorners(std::vector<glm::vec3>& corners) const {
    float aspectRatio = 1920.0f/1080.0f; // Ajusta esto según tu viewport
    float tanHalfFov = tan(glm::radians(zoom) / 2.0f);
    float nearH = getNearPlane() * tanHalfFov;
    float nearW = nearH * aspectRatio;
    float farH = getFarPlane() * tanHalfFov;
    float farW = farH * aspectRatio;

    // Near plane corners
    glm::vec3 fc = position + front * getNearPlane();
    corners.push_back(fc + (up * nearH) - (right * nearW));    // Near top left
    corners.push_back(fc + (up * nearH) + (right * nearW));    // Near top right
    corners.push_back(fc - (up * nearH) - (right * nearW));    // Near bottom left
    corners.push_back(fc - (up * nearH) + (right * nearW));    // Near bottom right

    // Far plane corners
    fc = position + front * getFarPlane();
    corners.push_back(fc + (up * farH) - (right * farW));      // Far top left
    corners.push_back(fc + (up * farH) + (right * farW));      // Far top right
    corners.push_back(fc - (up * farH) - (right * farW));      // Far bottom left
    corners.push_back(fc - (up * farH) + (right * farW));      // Far bottom right
}

void Camera::drawFrustumRays() const {
    std::vector<glm::vec3> corners;
    calculateFrustumCorners(corners);

    glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    // Líneas desde la cámara hasta el near plane
    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&position[0]);
        glVertex3fv(&corners[i][0]);
    }

    // Líneas desde la cámara hasta el far plane
    for (int i = 4; i < 8; ++i) {
        glVertex3fv(&position[0]);
        glVertex3fv(&corners[i][0]);
    }

    // Conectar near plane corners
    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&corners[i][0]);
        glVertex3fv(&corners[(i + 1) % 4][0]);
    }

    // Conectar far plane corners
    for (int i = 4; i < 8; ++i) {
        glVertex3fv(&corners[i][0]);
        glVertex3fv(&corners[4 + ((i - 4 + 1) % 4)][0]);
    }
    glEnd();

    glPopAttrib();
}
