#include "GameObject.h"
#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include <unordered_set>
#include <iostream>
// Inicializaci�n del contador est�tico para los IDs �nicos
int GameObject::nextId = 0;
std::unordered_set<std::string> GameObject::generatedNames;

GameObject::GameObject(const std::string& customName)
    : id(++nextId), scale(1.0f, 1.0f, 1.0f) { // Asigna una escala por defecto de (1,1,1)
    // Si no se proporciona un nombre, generamos uno �nico
    name = customName.empty() ? generateUniqueName() : customName;
}


GameObject::~GameObject() {
    generatedNames.erase(name); // Al destruir el objeto, eliminamos su nombre del conjunto
}

const std::string& GameObject::getName() const {
    return name;
}

void GameObject::setName(const std::string& newName) {
    // Aseguramos que el nuevo nombre tambi�n sea �nico antes de asignarlo
    if (generatedNames.find(newName) == generatedNames.end()) {
        generatedNames.erase(name); // Si el objeto ya tiene un nombre, lo eliminamos del conjunto
        name = newName;
        generatedNames.insert(name); // Insertamos el nuevo nombre
    }
}

bool GameObject::loadModel(const std::string& path) {
    std::cout << "Attempting to load model: " << path << std::endl;
    bool result = modelLoader.loadModel(path);
    if (result) {
        std::cout << "Model loaded: " << path << std::endl;
    } else {
        std::cout << "Failed to load model: " << path << std::endl;
    }
    return result;
}

void GameObject::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, scale.z);
    material.use(); // Aplica el material antes de dibujar el modelo

    modelLoader.drawModel();
    glPopMatrix();

    // Restablecer el estado de OpenGL
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Restablecer el color a blanco
}

// M�todos de transformaci�n
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

void GameObject::setRotation(const glm::vec3& rot) {
    rotation = rot;
}

glm::vec3 GameObject::getRotation() const {
    return rotation;
}

// M�todos de material
void GameObject::setMaterial(const Material& mat) {
    material = mat;
}

Material& GameObject::getMaterial() {
    return material;
}

// M�todo para generar un nombre �nico
std::string GameObject::generateUniqueName() {
    std::string uniqueName = "GameObject_" + std::to_string(nextId);
    while (generatedNames.find(uniqueName) != generatedNames.end()) {
        uniqueName = "GameObject_" + std::to_string(++nextId);
    }
    generatedNames.insert(uniqueName);
    return uniqueName;
}
