#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();
    bool loadModel(const std::string& path);
    void createCube(float size);
    void createSphere(float radius, int segments, int rings);
    void drawModel();

private:
    void drawNode(aiNode* node, const aiScene* scene);
    void drawPrimitive();

    const aiScene* scene;
    Assimp::Importer importer;

    struct Vertex {
        float x, y, z;
        float nx, ny, nz;
    };
    std::vector<Vertex> primitiveVertices;
};

#endif // MODELLOADER_H