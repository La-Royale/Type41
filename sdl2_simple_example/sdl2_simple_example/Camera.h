#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL_events.h>

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
};

#endif
