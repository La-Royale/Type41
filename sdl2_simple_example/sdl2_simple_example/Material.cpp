#include <GL/glew.h>
#include "Material.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <string>

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
    ILconst_string apath = reinterpret_cast<const ILconst_string>(path.c_str());

    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(apath)) {
        ILenum error = ilGetError();
        std::cerr << "Failed to load image: " << path << " - Error: " << iluErrorString(error) << std::endl;
        ilDeleteImages(1, &imageID);
        return false;
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    textureID = ilutGLBindTexImage();
    ilDeleteImages(1, &imageID);

    if (textureID == 0) {
        std::cerr << "Failed to bind texture: " << path << std::endl;
        return false;
    }

    hasTexture = true;
    return true;
}

void Material::use() const {
    if (hasTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(defaultColor.r, defaultColor.g, defaultColor.b);
    }
}

void Material::setDefaultColor(const glm::vec3& color) {
    defaultColor = color;
}