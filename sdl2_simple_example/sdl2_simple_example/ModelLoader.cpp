#include <GL/glew.h>
#include "ModelLoader.h"
#include "Logger.h"
#include <iostream>
#include <glm/glm.hpp> 
#include <cfloat>

ModelLoader::ModelLoader() : scene(nullptr) {}

ModelLoader::~ModelLoader() {
    scene = nullptr;
}

bool ModelLoader::loadModel(const std::string& path) {
    const aiScene* originalScene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_GenUVCoords);
    if (!originalScene || originalScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !originalScene->mRootNode) {
        Logger::GetInstance().Log("OBJECT INVALID TO ADD", WARNING);
        return false;
    }

    originalQuads.clear();
    for (unsigned int i = 0; i < originalScene->mNumMeshes; i++) {
        aiMesh* mesh = originalScene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace& face = mesh->mFaces[j];
            if (face.mNumIndices == 4) {
                originalQuads.push_back({ face.mIndices[0], face.mIndices[1], face.mIndices[2], face.mIndices[3] });
            }
        }
    }

    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::GetInstance().Log("OBJECT INVALID TO ADD", WARNING);
        return false;
    }

    minBound = glm::vec3(FLT_MAX);
    maxBound = glm::vec3(-FLT_MAX);
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D vertex = mesh->mVertices[j];
            minBound = glm::min(minBound, glm::vec3(vertex.x, vertex.y, vertex.z));
            maxBound = glm::max(maxBound, glm::vec3(vertex.x, vertex.y, vertex.z));
        }
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
        if (showBoundingBox) {
            drawBoundingBox();
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

void ModelLoader::setShowBoundingBox(bool show) {
    showBoundingBox = show;
}

// Dibuja las normales de las caras del modelo triangulado
void ModelLoader::drawTriangleNormals() {
    if (!scene) return;

    glPushMatrix();
    float scale = 0.2f; 
    glScalef(scale, scale, scale);

    glLineWidth(3.0f); 
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                unsigned int index1 = face.mIndices[k];
                unsigned int index2 = face.mIndices[(k + 1) % face.mNumIndices];
                aiVector3D vertex1 = mesh->mVertices[index1];
                aiVector3D vertex2 = mesh->mVertices[index2];

                glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                glVertex3f(vertex2.x, vertex2.y, vertex2.z);
            }
        }
    }
    glEnd();

    glLineWidth(1.0f); 
    glColor3f(1.0f, 1.0f, 1.0f); 

    glPopMatrix();
}


// Función para dibujar las normales de las caras, si el modelo tiene quads se dibujan las normales de los quads, si no, se dibujan las normales de las caras como la función anterior
void ModelLoader::drawFaceNormals() {
    if (!scene) return;

    glPushMatrix();
    float scale = 0.2f; 
    glScalef(scale, scale, scale);

    glLineWidth(3.0f); 
    glColor3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_LINES);
    if (!originalQuads.empty()) {
        for (const auto& quad : originalQuads) {
            for (unsigned int k = 0; k < 4; k++) {
                unsigned int index1 = quad[k];
                unsigned int index2 = quad[(k + 1) % 4];
                aiVector3D vertex1 = scene->mMeshes[0]->mVertices[index1];
                aiVector3D vertex2 = scene->mMeshes[0]->mVertices[index2];

                glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                glVertex3f(vertex2.x, vertex2.y, vertex2.z);
            }
        }
    } else {
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                aiFace& face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; k++) {
                    unsigned int index1 = face.mIndices[k];
                    unsigned int index2 = face.mIndices[(k + 1) % face.mNumIndices];
                    aiVector3D vertex1 = mesh->mVertices[index1];
                    aiVector3D vertex2 = mesh->mVertices[index2];

                    glVertex3f(vertex1.x, vertex1.y, vertex1.z);
                    glVertex3f(vertex2.x, vertex2.y, vertex2.z);
                }
            }
        }
    }
    glEnd();

    glLineWidth(1.0f); 
    glColor3f(1.0f, 1.0f, 1.0f); 

    glPopMatrix();
}

void ModelLoader::drawBoundingBox() {
    if (!scene) return;

    glPushMatrix();
    float scale = 0.2f;
    glScalef(scale, scale, scale);

    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 1.0f);

    glm::vec3 minBound(FLT_MAX), maxBound(-FLT_MAX);
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D vertex = mesh->mVertices[j];
            minBound = glm::min(minBound, glm::vec3(vertex.x, vertex.y, vertex.z));
            maxBound = glm::max(maxBound, glm::vec3(vertex.x, vertex.y, vertex.z));
        }
    }

    glBegin(GL_LINES);
    // Dibujar las líneas de la bounding box
    glVertex3f(minBound.x, minBound.y, minBound.z); glVertex3f(maxBound.x, minBound.y, minBound.z);
    glVertex3f(maxBound.x, minBound.y, minBound.z); glVertex3f(maxBound.x, maxBound.y, minBound.z);
    glVertex3f(maxBound.x, maxBound.y, minBound.z); glVertex3f(minBound.x, maxBound.y, minBound.z);
    glVertex3f(minBound.x, maxBound.y, minBound.z); glVertex3f(minBound.x, minBound.y, minBound.z);

    glVertex3f(minBound.x, minBound.y, maxBound.z); glVertex3f(maxBound.x, minBound.y, maxBound.z);
    glVertex3f(maxBound.x, minBound.y, maxBound.z); glVertex3f(maxBound.x, maxBound.y, maxBound.z);
    glVertex3f(maxBound.x, maxBound.y, maxBound.z); glVertex3f(minBound.x, maxBound.y, maxBound.z);
    glVertex3f(minBound.x, maxBound.y, maxBound.z); glVertex3f(minBound.x, minBound.y, maxBound.z);

    glVertex3f(minBound.x, minBound.y, minBound.z); glVertex3f(minBound.x, minBound.y, maxBound.z);
    glVertex3f(maxBound.x, minBound.y, minBound.z); glVertex3f(maxBound.x, minBound.y, maxBound.z);
    glVertex3f(maxBound.x, maxBound.y, minBound.z); glVertex3f(maxBound.x, maxBound.y, maxBound.z);
    glVertex3f(minBound.x, maxBound.y, minBound.z); glVertex3f(minBound.x, maxBound.y, maxBound.z);
    glEnd();

    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
}

glm::vec3 ModelLoader::getMinBound() const {
    return minBound;
}

glm::vec3 ModelLoader::getMaxBound() const {
    return maxBound;
}
