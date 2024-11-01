#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp> // Incluye glm para vectores
#include "ModelLoader.h"
#include "Material.h"
#include <string>

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
    void setMaterial(const Material& material);
    Material getMaterial() const;

private:
    ModelLoader modelLoader;
    glm::vec3 position; // Añade la variable de posición
    glm::vec3 scale;    // Añade la variable de escala
    Material material;  // Añade la variable de material
};

#endif // GAMEOBJECT_H