#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp> // Incluye glm para vectores
#include "ModelLoader.h"
#include "Material.h"
#include <string>

class GameObject {
public:
    GameObject(const std::string& name = "GameObject"); // Constructor con nombre
    ~GameObject();

    // Métodos para cargar y crear modelos
    bool loadModel(const std::string& path);
    void createCube(float size);
    void createSphere(float radius, int segments, int rings);
    void draw();

    // Métodos de transformación
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale() const;
    void setRotation(const glm::vec3& rotation);
    glm::vec3 getRotation() const;

    // Métodos de material
    void setMaterial(const Material& material);
    Material getMaterial() const;

    // Métodos para obtener y establecer el nombre
    std::string getName() const;
    void setName(const std::string& name);

private:
    std::string name;     // Añade la variable de nombre
    ModelLoader modelLoader;
    glm::vec3 position;   // Variable de posición
    glm::vec3 scale;      // Variable de escala
    glm::vec3 rotation;   // Variable de rotación
    Material material;    // Variable de material
};

#endif // GAMEOBJECT_H
