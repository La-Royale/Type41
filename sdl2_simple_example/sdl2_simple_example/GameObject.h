#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include "ModelLoader.h"
#include "Material.h"
#include <string>
#include <unordered_set>

class GameObject {
public:
    // Constructor con nombre opcional. Si no se proporciona, genera uno único.
    GameObject(const std::string& name = "");
    ~GameObject();

    const std::string& getName() const;
    void setName(const std::string& name);

    // Métodos para cargar y crear modelos
    bool loadModel(const std::string& path);
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
    Material& getMaterial();  // Return a reference to the material

private:
    std::string name;     // Nombre del objeto
    ModelLoader modelLoader;
    glm::vec3 position;   // Posición del objeto
    glm::vec3 scale;      // Escala del objeto
    glm::vec3 rotation;   // Rotación del objeto
    Material material;    // Material del objeto

    static int nextId;    // Contador estático de instancias
    static std::unordered_set<std::string> generatedNames; // Conjunto de nombres generados
    int id;               // ID único de cada GameObject

    // Método para asegurar que el nombre es único
    static std::string generateUniqueName();
};

#endif // GAMEOBJECT_H
