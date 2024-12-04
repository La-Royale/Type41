#include "GameObject.h"
#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include <unordered_set>
#include <iostream>
#include <cfloat>

// Inicialización del contador estático para los IDs únicos
int GameObject::nextId = 0;
std::unordered_set<std::string> GameObject::generatedNames;

std::unique_ptr<GameObject> GameObject::clone() const {
    std::unique_ptr<GameObject> cloned = std::make_unique<GameObject>(name, isStatic);

    // Copiar otros miembros de datos que deban ser duplicados
    cloned->position = this->position;
    cloned->rotation = this->rotation;
    cloned->scale = this->scale;
    cloned->material = this->material;

    // Aquí puedes hacer lo mismo con otros recursos si es necesario.

    return cloned;
}

GameObject::GameObject(const std::string& customName, bool isStatic)
    : id(++nextId), scale(1.0f, 1.0f, 1.0f), isStatic(isStatic) { // Asigna una escala por defecto de (1,1,1)
    // Si no se proporciona un nombre, generamos uno único
    name = customName.empty() ? generateUniqueName() : customName;
}

GameObject::~GameObject() {
    generatedNames.erase(name); // Al destruir el objeto, eliminamos su nombre del conjunto
}

const std::string& GameObject::getName() const {
    return name;
}

void GameObject::setName(const std::string& newName) {
    // Aseguramos que el nuevo nombre también sea único antes de asignarlo
    if (generatedNames.find(newName) == generatedNames.end()) {
        generatedNames.erase(name); // Si el objeto ya tiene un nombre, lo eliminamos del conjunto
        name = newName;
        generatedNames.insert(name); // Insertamos el nuevo nombre
    }
}

bool GameObject::loadModel(const std::string& path) {
    bool result = modelLoader.loadModel(path);
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

// Métodos de transformación
void GameObject::setPosition(const glm::vec3& pos) {
    if (!isStatic) {
        position = pos;
    }
}

glm::vec3 GameObject::getPosition() const {
    return position;
}

void GameObject::setScale(const glm::vec3& scl) {
    if (!isStatic) {
        scale = scl;
    }
}

glm::vec3 GameObject::getScale() const {
    return scale;
}

void GameObject::setRotation(const glm::vec3& rot) {
    if (!isStatic) {
        rotation = rot;
    }
}

glm::vec3 GameObject::getRotation() const {
    return rotation;
}

// Métodos de material
void GameObject::setMaterial(const Material& mat) {
    material = mat;
}

Material& GameObject::getMaterial() {
    return material;
}

ModelLoader& GameObject::getModelLoader() {
    return modelLoader; // Devuelve el ModelLoader asociado al GameObject
}

// Método para generar un nombre único
std::string GameObject::generateUniqueName() {
    std::string uniqueName = "GameObject_" + std::to_string(nextId);
    while (generatedNames.find(uniqueName) != generatedNames.end()) {
        uniqueName = "GameObject_" + std::to_string(++nextId);
    }
    generatedNames.insert(uniqueName);
    return uniqueName;
}

glm::vec3 GameObject::getMeshSize() const {
    const aiScene* scene = modelLoader.getScene();
    glm::vec3 minBound(FLT_MAX), maxBound(-FLT_MAX);

    if (scene) {
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                aiVector3D vertex = mesh->mVertices[j];
                minBound = glm::min(minBound, glm::vec3(vertex.x, vertex.y, vertex.z));
                maxBound = glm::max(maxBound, glm::vec3(vertex.x, vertex.y, vertex.z));
            }
        }
    }

    return maxBound - minBound;
}

bool GameObject::getStatic() const {
    return isStatic;
}

void GameObject::setStatic(bool isStatic) {
    this->isStatic = isStatic;
}