#include <GL/glew.h>
#include "ModelLoader.h"
#include "Logger.h"
#include <iostream>

ModelLoader::ModelLoader() : scene(nullptr) {}

ModelLoader::~ModelLoader() {
    scene = nullptr;
}

bool ModelLoader::loadModel(const std::string& path) {
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::GetInstance().Log("OBJECT INVALID TO ADD", WARNING);
        return false;
    }

    Logger::GetInstance().Log("OBJECT WAS SUCCESFULLY ADDED", INFO);
    primitiveVertices.clear();
    return true;
}

void ModelLoader::drawModel() {
    if (scene) {
        drawNode(scene->mRootNode, scene);

        if (showTriangleNormals) {
            drawTriangleNormals();
        }
        if (showFaceNormals) {
            drawFaceNormals();
        }
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

void ModelLoader::setShowTriangleNormals(bool show) {
    showTriangleNormals = show;
}

void ModelLoader::setShowFaceNormals(bool show) {
    showFaceNormals = show;
}

void ModelLoader::drawTriangleNormals() {
    if (!scene) return;

    glBegin(GL_LINES);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                unsigned int index = face.mIndices[k];
                aiVector3D vertex = mesh->mVertices[index];
                aiVector3D normal = mesh->mNormals[index];

                glVertex3f(vertex.x, vertex.y, vertex.z);
                glVertex3f(vertex.x + normal.x * 0.2f, vertex.y + normal.y * 0.2f, vertex.z + normal.z * 0.2f);
            }
        }
    }
    glEnd();
}

void ModelLoader::drawFaceNormals() {
    if (!scene) return;

    glBegin(GL_LINES);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace& face = mesh->mFaces[j];
            aiVector3D centroid(0.0f, 0.0f, 0.0f);

            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                unsigned int index = face.mIndices[k];
                centroid += mesh->mVertices[index];
            }
            centroid /= (float)face.mNumIndices;

            aiVector3D faceNormal(0.0f, 0.0f, 0.0f);
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                unsigned int index = face.mIndices[k];
                faceNormal += mesh->mNormals[index];
            }
            faceNormal.Normalize();

            glVertex3f(centroid.x, centroid.y, centroid.z);
            glVertex3f(centroid.x-10 + faceNormal.x-10 * 0.2f, centroid.y-10 + faceNormal.y-10 * 0.2f, centroid.z-10 + faceNormal.z-10 * 0.2f);
        }
    }
    glEnd();
}
