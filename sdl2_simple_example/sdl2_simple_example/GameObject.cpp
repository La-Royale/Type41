#define GLM_ENABLE_EXPERIMENTAL

#include "GameObject.h"
#include <GL/glew.h>
#include <unordered_set>
#include <iostream>
#include <cfloat>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

int GameObject::nextId = 0;
std::unordered_set<std::string> GameObject::generatedNames;

std::unique_ptr<GameObject> GameObject::clone() const {
    std::unique_ptr<GameObject> cloned = std::make_unique<GameObject>(name, isStatic);

    cloned->position = position;
    cloned->rotation = rotation;
    cloned->scale = scale;
    cloned->material = material;

    for (GameObject* child : children) {
        cloned->addChild(child->clone().release());
    }

    return cloned;
}

GameObject::GameObject(const std::string& customName, bool isStatic)
    : id(++nextId), scale(1.0f, 1.0f, 1.0f), isStatic(isStatic) {
    name = customName.empty() ? generateUniqueName() : customName;
}

GameObject::~GameObject() {
    generatedNames.erase(name);
}

void GameObject::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x, scale.y, scale.z);
    material.use();

    modelLoader.drawModel();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void GameObject::updateParentTransform() {
    if (parent && !parent->isUpdating) {
        parent->updateTransform();
    }
}

void GameObject::updateTransform() {
    if (isUpdating) {
        return;
    }

    isUpdating = true;

    // Si el objeto tiene un padre, actualiza su transformación global con respecto al padre
    if (parent) {
        parent->updateTransform();
        globalTransform = parent->getGlobalTransform() * getLocalTransform();
    }
    else {
        globalTransform = getLocalTransform();
    }

    // Actualiza la transformación de todos los hijos de este objeto
    updateChildrenTransform();

    isUpdating = false;
}

void GameObject::updateChildrenTransform() {
    for (GameObject* child : children) {
        child->updateTransform();  // Recursión para actualizar a cada hijo
    }
}

glm::vec3 GameObject::getPosition() const { return position; }
void GameObject::setPosition(const glm::vec3& pos) {
    position = pos;
    updateTransform();
}

glm::vec3 GameObject::getRotation() const { return rotation; }
void GameObject::setRotation(const glm::vec3& rot) {
    rotation = rot;
    updateTransform();
}

glm::vec3 GameObject::getScale() const { return scale; }
void GameObject::setScale(const glm::vec3& scl) {
    scale = scl;
    updateTransform();
}

GameObject* GameObject::getParent() const { return parent; }

void GameObject::setParent(GameObject* newParent) {
    if (parent == newParent) {
        return;
    }

    GameObject* temp = newParent;
    while (temp) {
        if (temp == this) {
            return;
        }
        temp = temp->getParent();
    }

    if (parent) {
        parent->removeChild(this);
    }

    parent = newParent;

    if (parent) {
        parent->addChild(this);
    }

    updateTransform();
}

const std::vector<GameObject*>& GameObject::getChildren() const {
    return children;
}

void GameObject::addChild(GameObject* child) {
    if (child && std::find(children.begin(), children.end(), child) == children.end()) {
        children.push_back(child);
    }
}

void GameObject::removeChild(GameObject* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

glm::mat4 GameObject::getGlobalTransform() const {
    glm::mat4 localTransform = getLocalTransform();

    if (parent) {
        return parent->getGlobalTransform() * localTransform;
    }
    else {
        return localTransform;
    }
}

glm::mat4 GameObject::getLocalTransform() const {
    glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::translate(transform, position);

    glm::quat quaternionRotation = glm::quat(glm::radians(rotation));
    transform *= glm::mat4_cast(quaternionRotation);

    transform = glm::scale(transform, scale);

    return transform;
}

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

glm::vec3 GameObject::getMinBound() const {
    return modelLoader.getMinBound();
}

glm::vec3 GameObject::getMaxBound() const {
    return modelLoader.getMaxBound();
}

glm::vec3 GameObject::getGlobalMinBound() const {
    glm::vec3 localMin = modelLoader.getMinBound();
    glm::vec3 globalMin = position + glm::vec3(scale.x * localMin.x, scale.y * localMin.y, scale.z * localMin.z);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    globalMin = glm::vec3(rotationMatrix * glm::vec4(globalMin, 1.0f));

    return globalMin;
}

glm::vec3 GameObject::getGlobalMaxBound() const {
    glm::vec3 localMax = modelLoader.getMaxBound();
    glm::vec3 globalMax = position + glm::vec3(scale.x * localMax.x, scale.y * localMax.y, scale.z * localMax.z);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    globalMax = glm::vec3(rotationMatrix * glm::vec4(globalMax, 1.0f));

    return globalMax;
}

void GameObject::update(float deltaTime) {}

void GameObject::setName(const std::string& newName) {
    if (generatedNames.find(newName) == generatedNames.end()) {
        generatedNames.erase(name);
        name = newName;
        generatedNames.insert(name);
    }
}

void GameObject::setMaterial(const Material& material) {
    this->material = material;
}

const std::string& GameObject::getName() const {
    return this->name;
}

ModelLoader& GameObject::getModelLoader() {
    return modelLoader;
}

Material& GameObject::getMaterial() {
    return material;
}

bool GameObject::loadModel(const std::string& path) {
    bool result = modelLoader.loadModel(path);
    return result;
}

void GameObject::removeFromParent() {
    if (parent) {
        parent->removeChild(this);
        parent = nullptr;
    }
}