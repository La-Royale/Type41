#ifndef MATERIAL_H
#define MATERIAL_H

//#include <GL/glew.h> // Incluye GLEW para definir GLuint
#include <glm/glm.hpp>
#include <string>

class Material {
public:
    Material();
    ~Material();
    bool loadTexture(const std::string& path);
    void use() const;
    void setDefaultColor(const glm::vec3& color);

private:
    unsigned int textureID;
    bool hasTexture;
    glm::vec3 defaultColor;
};

#endif // MATERIAL_H