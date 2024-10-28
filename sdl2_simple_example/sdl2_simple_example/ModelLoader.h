// ModelLoader.h

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();

    bool loadModel(const std::string& path);
    void drawModel();

    // Métodos para generar primitivas
    void createCube(float size = 1.0f);
    void createSphere(float radius = 1.0f, int segments = 16, int rings = 16);
    // Agrega más métodos de primitivas si es necesario

private:
    const aiScene* scene = nullptr;
    Assimp::Importer importer;

    struct Vertex {
        float x, y, z;
        float nx, ny, nz;
    };

    std::vector<Vertex> primitiveVertices;
    void drawPrimitive();
    void drawNode(aiNode* node, const aiScene* scene);
};

#endif
