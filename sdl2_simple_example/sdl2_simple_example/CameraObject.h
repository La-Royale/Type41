#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H

#include "GameObject.h"
#include <vector>
#include <glm/glm.hpp>

class CameraObject : public GameObject {
public:
    CameraObject(const std::string& name = "Camera");
    virtual void draw() override;

    float getFov() const { return fov; }
    float getNearPlane() const { return nearPlane; }
    float getFarPlane() const { return farPlane; }
    float getAspectRatio() const { return aspectRatio; }

    void setFov(float value) { fov = value; }
    void setNearPlane(float value) { nearPlane = value; }
    void setFarPlane(float value) { farPlane = value; }
    void setAspectRatio(float value) { aspectRatio = value; }

private:
    void calculateFrustumCorners(std::vector<glm::vec3>& corners) const;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 20.0f;
    float aspectRatio = 16.0f/9.0f;
};

#endif // CAMERAOBJECT_H
