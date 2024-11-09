#include <GL/glew.h>
#include "Material.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Logger.h"

Material::Material() : textureID(0), hasTexture(false), defaultColor(1.0f, 0.0f, 1.0f) {
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);
}

Material::~Material() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

bool Material::loadTexture(const std::string& path) {
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    ILconst_string apath = reinterpret_cast<const ILconst_string>(path.c_str());
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(apath)) {
        ILenum error = ilGetError();
        ilDeleteImages(1, &imageID);
        return false;
    }

    textureWidth = ilGetInteger(IL_IMAGE_WIDTH);
    textureHeight = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ilDeleteImages(1, &imageID);

    int glWidth, glHeight;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &glWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &glHeight);

    texturePath = path;
    hasTexture = (textureID != 0);
    Logger::GetInstance().Log(hasTexture ? "TEXTURE WAS SUCCESSFULLY ADDED" : "OBJECT INVALID TO ADD", hasTexture ? INFO : WARNING);

    return hasTexture;
}

GLuint Material::generateCheckeredTexture(int width, int height) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    std::vector<unsigned char> data(width * height * 4);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char r = ((x / 32) + (y / 32)) % 2 == 0 ? 255 : 0;
            unsigned char g = r;
            unsigned char b = r;
            unsigned char a = 255;
            data[(y * width + x) * 4 + 0] = r;
            data[(y * width + x) * 4 + 1] = g;
            data[(y * width + x) * 4 + 2] = b;
            data[(y * width + x) * 4 + 3] = a;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureID;
}

void Material::use() const {
    if (hasTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(defaultColor.r, defaultColor.g, defaultColor.b);
    }
}

void Material::setDefaultColor(const glm::vec3& color) {
    defaultColor = color;
}
