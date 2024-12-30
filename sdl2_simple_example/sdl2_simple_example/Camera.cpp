#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

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
    glm::mat4 proj = getProjectionMatrix(1920.0f/1080.0f);
    glm::mat4 view = getViewMatrix();
    glm::mat4 vp = proj * view;

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

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getRayDirection() const {
    return front;
}

void Camera::calculateFrustumCorners(std::vector<glm::vec3>& corners) const {
    float aspectRatio = 1920.0f/1080.0f; 
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

glm::vec3 Camera::screenToWorldRay(float screenX, float screenY, float screenWidth, float screenHeight) {
    // Convert screen coordinates to normalized device coordinates (-1 to 1)
    float x = (2.0f * screenX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * screenY) / screenHeight;
    
    // Get inverse view-projection matrix
    glm::mat4 invVP = getInverseViewProjection(screenWidth, screenHeight);
    
    // Calculate ray direction in world space
    glm::vec4 rayStart = invVP * glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 rayEnd = invVP * glm::vec4(x, y, 1.0f, 1.0f);
    
    rayStart /= rayStart.w;
    rayEnd /= rayEnd.w;
    
    return glm::normalize(glm::vec3(rayEnd - rayStart));
}

glm::mat4 Camera::getInverseViewProjection(float screenWidth, float screenHeight) const {
    glm::mat4 projection = getProjectionMatrix(screenWidth / screenHeight);
    glm::mat4 view = getViewMatrix();
    return glm::inverse(projection * view);
}
