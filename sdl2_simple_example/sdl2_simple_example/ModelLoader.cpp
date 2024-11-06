#include <GL/glew.h>
#include "ModelLoader.h"
#include "ConsolePanel.h"
#include <cmath>
#include <iostream>

ModelLoader::ModelLoader() : scene(nullptr) {}

ModelLoader::~ModelLoader() {
    scene = nullptr;
}

bool ModelLoader::loadModel(const std::string& path) {
    // Log de intento de carga
    ConsolePanel console;
    console.Log(("Attempting to load model from path: " + path).c_str(), INFO);

    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        // Log de error
        console.Log(("Failed to load model: " + path).c_str(), ERROR);
        return false;
    }

    // Log de éxito
    console.Log(("Model loaded successfully: " + path).c_str(), INFO);

    primitiveVertices.clear();
    return true;
}
void ModelLoader::drawModel() {
    if (scene) {
        drawNode(scene->mRootNode, scene);
    }
    else if (!primitiveVertices.empty()) {
        drawPrimitive();
    }
}

void ModelLoader::drawNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        glPushMatrix();
        float scale = 0.2f;
        glScalef(scale, scale, scale);

        glBegin(GL_TRIANGLES);
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                unsigned int index = face.mIndices[k];
                if (mesh->HasNormals()) {
                    glNormal3fv(&mesh->mNormals[index].x);
                }
                if (mesh->HasTextureCoords(0)) {
                    glTexCoord2f(mesh->mTextureCoords[0][index].x, -mesh->mTextureCoords[0][index].y);
                }
                glVertex3fv(&mesh->mVertices[index].x);
            }
        }
        glEnd();

        glPopMatrix();
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        drawNode(node->mChildren[i], scene);
    }
}

void ModelLoader::drawPrimitive() {
    glBegin(GL_TRIANGLES);
    for (const auto& vertex : primitiveVertices) {
        glNormal3f(vertex.nx, vertex.ny, vertex.nz);
        glTexCoord2f(vertex.u, vertex.v);
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}

const aiScene* ModelLoader::getScene() const {
    return scene;
}