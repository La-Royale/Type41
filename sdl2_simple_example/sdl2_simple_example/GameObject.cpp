#include "GameObject.h"

GameObject::GameObject() {}

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
    modelLoader.drawModel();
}