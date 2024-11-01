#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ModelLoader.h"
#include <string>
#include <glm/glm.hpp> // Incluye glm para vectores

class GameObject {
public:
    GameObject();
    ~GameObject();
    bool loadModel(const std::string& path);
    void createCube(float size);
    void createSphere(float radius, int segments, int rings);
    void draw();
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale() const;

private:
    ModelLoader modelLoader;
    glm::vec3 position; // Añade la variable de posición
    glm::vec3 scale;    // Añade la variable de escala
};

#endif // GAMEOBJECT_H