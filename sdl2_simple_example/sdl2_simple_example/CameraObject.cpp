#include "CameraObject.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

CameraObject::CameraObject(const std::string& name) : GameObject(name) {}

void CameraObject::calculateFrustumCorners(std::vector<glm::vec3>& corners) const {
    float tanHalfFov = tan(glm::radians(fov) / 2.0f);
    float nearH = nearPlane * tanHalfFov;
    float nearW = nearH * aspectRatio;
    float farH = farPlane * tanHalfFov;
    float farW = farH * aspectRatio;

    glm::vec3 front = glm::normalize(glm::vec3(
        -sin(glm::radians(getRotation().y)) * cos(glm::radians(getRotation().x)),
        sin(glm::radians(getRotation().x)),
        -cos(glm::radians(getRotation().y)) * cos(glm::radians(getRotation().x))
    ));

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    // Near plane corners
    glm::vec3 fc = getPosition() + front * nearPlane;
    corners.push_back(fc + (up * nearH) - (right * nearW));    // Near top left
    corners.push_back(fc + (up * nearH) + (right * nearW));    // Near top right
    corners.push_back(fc - (up * nearH) - (right * nearW));    // Near bottom left
    corners.push_back(fc - (up * nearH) + (right * nearW));    // Near bottom right

    // Far plane corners
    fc = getPosition() + front * farPlane;
    corners.push_back(fc + (up * farH) - (right * farW));      // Far top left
    corners.push_back(fc + (up * farH) + (right * farW));      // Far top right
    corners.push_back(fc - (up * farH) - (right * farW));      // Far bottom left
    corners.push_back(fc - (up * farH) + (right * farW));      // Far bottom right
}

void CameraObject::draw() {
    std::vector<glm::vec3> corners;
    calculateFrustumCorners(corners);

    glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_LINES);

    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&getPosition()[0]);
        glVertex3fv(&corners[i][0]);
    }

    for (int i = 4; i < 8; ++i) {
        glVertex3fv(&getPosition()[0]);
        glVertex3fv(&corners[i][0]);
    }

    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&corners[i][0]);
        glVertex3fv(&corners[(i + 1) % 4][0]);
    }

    for (int i = 4; i < 8; ++i) {
        glVertex3fv(&corners[i][0]);
        glVertex3fv(&corners[4 + ((i - 4 + 1) % 4)][0]);
    }
    glEnd();

    glPopAttrib();

    GameObject::draw();
}
