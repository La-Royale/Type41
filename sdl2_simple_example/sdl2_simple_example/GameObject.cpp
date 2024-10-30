#include "GameObject.h"
#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include <GL/gl.h>   // Incluye OpenGL

GameObject::GameObject() : position(0.0f, 0.0f, 0.0f) {}

GameObject::~GameObject() {}

bool GameObject::loadModel(const std::string& path) {
    return modelLoader.loadModel(path);
}

void GameObject::createCube(float size) {
    modelLoader.createCube(size);
}

void GameObject::createSphere(float radius, int segments, int rings) {
    modelLoader.createSphere(radius, segments, rings);
}

void GameObject::draw() {
    // Aplica la transformación de posición antes de dibujar
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    modelLoader.drawModel();
    glPopMatrix();
}

void GameObject::setPosition(const glm::vec3& pos) {
    position = pos;
}

glm::vec3 GameObject::getPosition() const {
    return position;
}