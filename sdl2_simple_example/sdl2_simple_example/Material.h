#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <string>

class Material {
public:
    Material();
    ~Material();
    bool loadTexture(const std::string& path);
    void use() const;
    void setDefaultColor(const glm::vec3& color);

    unsigned int getTextureID() const { return textureID; }
    bool hasLoadedTexture() const { return hasTexture; }

    int getTextureWidth() const { return textureWidth; }
    int getTextureHeight() const { return textureHeight; }


    const std::string& getTexturePath() const { return texturePath; }
    void setTexture(unsigned int texture) { textureID = texture; }

    static unsigned int generateCheckeredTexture(int width, int height);

private:

    unsigned int textureID;
    bool hasTexture;
    glm::vec3 defaultColor;
    std::string texturePath;

    int textureWidth, textureHeight;
};

#endif // MATERIAL_H