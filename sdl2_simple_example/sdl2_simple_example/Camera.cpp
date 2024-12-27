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
    glm::mat4 projView = getProjectionMatrix(1.0f) * getViewMatrix();
    frustumPlanes[0] = glm::vec4(projView[0][3] + projView[0][0], projView[1][3] + projView[1][0], projView[2][3] + projView[2][0], projView[3][3] + projView[3][0]); // Left
    frustumPlanes[1] = glm::vec4(projView[0][3] - projView[0][0], projView[1][3] - projView[1][0], projView[2][3] - projView[2][0], projView[3][3] - projView[3][0]); // Right
    frustumPlanes[2] = glm::vec4(projView[0][3] + projView[0][1], projView[1][3] + projView[1][1], projView[2][3] + projView[2][1], projView[3][3] + projView[3][1]); // Bottom
    frustumPlanes[3] = glm::vec4(projView[0][3] - projView[0][1], projView[1][3] - projView[1][1], projView[2][3] - projView[2][1], projView[3][3] - projView[3][1]); // Top
    frustumPlanes[4] = glm::vec4(projView[0][3] + projView[0][2], projView[1][3] + projView[1][2], projView[2][3] + projView[2][2], projView[3][3] + projView[3][2]); // Near
    frustumPlanes[5] = glm::vec4(projView[0][3] - projView[0][2], projView[1][3] - projView[1][2], projView[2][3] - projView[2][2], projView[3][3] - projView[3][2]); // Far

    for (int i = 0; i < 6; ++i) {
        float length = glm::length(glm::vec3(frustumPlanes[i]));
        frustumPlanes[i] /= length;
    }
}

bool Camera::isBoxInFrustum(const glm::vec3& minBound, const glm::vec3& maxBound) const {
    for (int i = 0; i < 6; ++i) {
        glm::vec3 positiveVertex = minBound;
        glm::vec3 negativeVertex = maxBound;

        if (frustumPlanes[i].x >= 0) {
            positiveVertex.x = maxBound.x;
            negativeVertex.x = minBound.x;
        }
        else {
            positiveVertex.x = minBound.x;
            negativeVertex.x = maxBound.x;
        }

        if (frustumPlanes[i].y >= 0) {
            positiveVertex.y = maxBound.y;
            negativeVertex.y = minBound.y;
        }
        else {
            positiveVertex.y = minBound.y;
            negativeVertex.y = maxBound.y;
        }

        if (frustumPlanes[i].z >= 0) {
            positiveVertex.z = maxBound.z;
            negativeVertex.z = minBound.z;
        }
        else {
            positiveVertex.z = minBound.z;
            negativeVertex.z = maxBound.z;
        }

        if (glm::dot(glm::vec3(frustumPlanes[i]), positiveVertex) + frustumPlanes[i].w < 0) {
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

Camera::Ray Camera::GenerateRay(int mouseX, int mouseY, int screenWidth, int screenHeight, float aspectRatio) const {
    glm::vec2 ndc = ScreenToNDC(mouseX, mouseY, screenWidth, screenHeight);

    glm::vec4 rayClip(ndc.x, ndc.y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(getProjectionMatrix(aspectRatio)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::vec3(glm::inverse(getViewMatrix()) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    return Ray(position, rayWorld);
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getRayDirection() const {
    return front;
}
