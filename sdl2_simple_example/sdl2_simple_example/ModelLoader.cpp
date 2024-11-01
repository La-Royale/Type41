#include <GL/glew.h>
#include "ModelLoader.h"
#include <cmath>
#include <iostream>

ModelLoader::ModelLoader() : scene(nullptr) {}

ModelLoader::~ModelLoader() {
    scene = nullptr;
}

bool ModelLoader::loadModel(const std::string& path) {
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model: " << path << std::endl;
        return false;
    }
    primitiveVertices.clear();
    return true;
}

void ModelLoader::createCube(float size) {
    primitiveVertices = {
        {-size, -size, -size, 0, 0, -1, 0, 0}, {size, -size, -size, 0, 0, -1, 1, 0}, {size, size, -size, 0, 0, -1, 1, 1},
        {-size, -size, -size, 0, 0, -1, 0, 0}, {size, size, -size, 0, 0, -1, 1, 1}, {-size, size, -size, 0, 0, -1, 0, 1},
        // Añade los otros lados del cubo con sus UVs correspondientes
    };
}

void ModelLoader::createSphere(float radius, int segments, int rings) {
    primitiveVertices.clear();
    for (int i = 0; i <= rings; i++) {
        float theta = i * 3.14f / rings;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= segments; j++) {
            float phi = j * 2 * 3.14f / segments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertex vertex;
            vertex.x = radius * sinTheta * cosPhi;
            vertex.y = radius * cosTheta;
            vertex.z = radius * sinTheta * sinPhi;
            vertex.nx = vertex.x;
            vertex.ny = vertex.y;
            vertex.nz = vertex.z;
            vertex.u = (float)j / segments;
            vertex.v = (float)i / rings;

            primitiveVertices.push_back(vertex);
        }
    }
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