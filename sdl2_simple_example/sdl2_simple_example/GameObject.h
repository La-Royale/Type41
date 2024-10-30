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

private:
    ModelLoader modelLoader;
    glm::vec3 position; // Añade la variable de posición
};

#endif // GAMEOBJECT_H