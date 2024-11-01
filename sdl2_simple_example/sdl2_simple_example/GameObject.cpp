#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include "GameObject.h"

GameObject::GameObject() : position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

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
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(scale.x, scale.y, scale.z);
    material.use(); // Aplica el material antes de dibujar el modelo
    modelLoader.drawModel();
    glPopMatrix();
}

void GameObject::setPosition(const glm::vec3& pos) {
    position = pos;
}

glm::vec3 GameObject::getPosition() const {
    return position;
}

void GameObject::setScale(const glm::vec3& scl) {
    scale = scl;
}

glm::vec3 GameObject::getScale() const {
    return scale;
}

void GameObject::setMaterial(const Material& mat) {
    material = mat;
}

Material GameObject::getMaterial() const {
    return material;
}