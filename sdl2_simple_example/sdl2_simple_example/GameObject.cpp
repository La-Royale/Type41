#define GLM_ENABLE_EXPERIMENTAL

#include "GameObject.h"
#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include <unordered_set>
#include <iostream>
#include <cfloat>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Para translate, rotate y scale
#include <glm/gtx/euler_angles.hpp>     // Para matrices de rotación con ángulos de Euler

// Inicialización del contador estático para los IDs únicos
int GameObject::nextId = 0;
std::unordered_set<std::string> GameObject::generatedNames;

std::unique_ptr<GameObject> GameObject::clone() const {
    std::unique_ptr<GameObject> cloned = std::make_unique<GameObject>(name, isStatic);

    // Copiar datos de la transformación
    cloned->position = position;
    cloned->rotation = rotation;
    cloned->scale = scale;
    cloned->material = material;

    // Copiar la jerarquía (relación padre-hijo)
    for (GameObject* child : children) {
        cloned->addChild(child->clone().release());  // Clonamos y agregamos los hijos
    }

    // Si el objeto tiene un padre, debemos asegurarnos de que la jerarquía se mantenga
    if (parent) {
        // Asegúrate de no copiar la relación jerárquica al clon directamente
        // cloned->setParent(parent);  // No se debería clonar la relación padre-hijo aquí
    }

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

void GameObject::updateParentTransform() {
    if (parent && !parent->isUpdating) {
        parent->updateTransform();  // Aseguramos que la transformación del padre esté actualizada
    }
}

void GameObject::updateTransform() {
    if (isUpdating) {
        return;  // Si ya estamos actualizando, salimos para evitar recursión infinita
    }

    isUpdating = true;  // Marcamos que estamos actualizando este objeto

    if (parent) {
        parent->updateTransform();  // Asegura que el padre esté actualizado
    }

    globalTransform = getGlobalTransform();  // Actualizamos la transformación global

    updateChildrenTransform();  // Propagamos las transformaciones a los hijos

    isUpdating = false;  // Marcamos que hemos terminado de actualizar
}


void GameObject::updateChildrenTransform() {
    for (GameObject* child : children) {
        child->updateTransform();  // Recursivamente actualizamos la transformación de cada hijo
    }
}

// Métodos de posición, rotación y escala
glm::vec3 GameObject::getPosition() const { return position; }
void GameObject::setPosition(const glm::vec3& pos) {
    position = pos;
    updateTransform();  // Propagamos el cambio al padre y a los hijos
}

glm::vec3 GameObject::getRotation() const { return rotation; }
void GameObject::setRotation(const glm::vec3& rot) {
    rotation = rot;
    updateTransform(); // Notificar al padre de cualquier cambio
}

glm::vec3 GameObject::getScale() const { return scale; }
void GameObject::setScale(const glm::vec3& scl) {
    scale = scl;
    updateTransform(); // Notificar al padre de cualquier cambio
}

// Métodos de jerarquía padre-hijo
GameObject* GameObject::getParent() const { return parent; }

void GameObject::setParent(GameObject* newParent) {
    if (parent == newParent) {
        return; // Si el nuevo padre es el mismo que el actual, no hacemos nada
    }

    // Evitar ciclos: No podemos hacer un objeto hijo de sí mismo
    GameObject* temp = newParent;
    while (temp) {
        if (temp == this) {
            std::cerr << "No se puede hacer un objeto hijo de sí mismo." << std::endl;
            return; // Evitamos el ciclo
        }
        temp = temp->getParent();
    }

    // Si ya tenía un padre, eliminar de sus hijos
    if (parent) {
        parent->removeChild(this);
    }

    parent = newParent; // Asignar el nuevo padre

    // Si hay un nuevo padre, agregar como hijo y notificar la jerarquía
    if (parent) {
        parent->addChild(this);
    }

    // Actualizar transformaciones
    updateTransform();
}



const std::vector<GameObject*>& GameObject::getChildren() const {
    return children;
}

void GameObject::addChild(GameObject* child) {
    if (child && std::find(children.begin(), children.end(), child) == children.end()) {
        children.push_back(child);  // Añadimos el hijo si no está ya en la lista
    }
}

void GameObject::removeChild(GameObject* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

// Cálculo de la transformación global
glm::mat4 GameObject::getGlobalTransform() const {
    glm::mat4 localTransform = glm::mat4(1.0f);  // Inicia con una matriz identidad

    // Aplica las transformaciones locales
    localTransform = glm::translate(localTransform, position);
    glm::quat quaternionRotation = glm::quat(glm::radians(rotation));
    localTransform *= glm::mat4_cast(quaternionRotation);
    localTransform = glm::scale(localTransform, scale);

    // Si tiene un padre, combinamos la transformación del padre con la transformación local
    if (parent) {
        return parent->getGlobalTransform() * localTransform;  // Propagamos la transformación global del padre
    }
    else {
        return localTransform;  // Si no tiene padre, la transformación es solo la local
    }
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

glm::vec3 GameObject::getMinBound() const {
    return modelLoader.getMinBound();
}

glm::vec3 GameObject::getMaxBound() const {
    return modelLoader.getMaxBound();
}

glm::vec3 GameObject::getGlobalMinBound() const {
    glm::vec3 localMin = modelLoader.getMinBound();
    glm::vec3 globalMin = position + glm::vec3(scale.x * localMin.x, scale.y * localMin.y, scale.z * localMin.z);

    // Aplicar rotación
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

    // Aplicar rotación
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    globalMax = glm::vec3(rotationMatrix * glm::vec4(globalMax, 1.0f));

    return globalMax;
}

void GameObject::update(float deltaTime) {}

void GameObject::setName(const std::string& newName) {
    // Aseguramos que el nuevo nombre también sea único antes de asignarlo
    if (generatedNames.find(newName) == generatedNames.end()) {
        generatedNames.erase(name); // Si el objeto ya tiene un nombre, lo eliminamos del conjunto
        name = newName;
        generatedNames.insert(name); // Insertamos el nuevo nombre
    }
}

void GameObject::setMaterial(const Material& material) {
    this->material = material;  // Implementación real
}

const std::string& GameObject::getName() const {
    return this->name;
}

ModelLoader& GameObject::getModelLoader() {
    return modelLoader;  // Aquí 'modelLoader' debería ser un miembro de la clase
}

Material& GameObject::getMaterial() {
    return material;  // Aquí 'material' debería ser un miembro de la clase
}

bool GameObject::loadModel(const std::string& path) {
    bool result = modelLoader.loadModel(path);
    return result;
}

void GameObject::removeFromParent() {
    if (parent) {
        parent->removeChild(this);
        parent = nullptr;  // Desconectamos al objeto de su padre
    }
}
