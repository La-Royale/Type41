#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    float x, y, z;    // Posición
    float nx, ny, nz; // Normales
    float u, v;       // Coordenadas UV
};

class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();
    bool loadModel(const std::string& path);
    void drawModel();
    const aiScene* getScene() const;

    bool isShowingTriangleNormals() const { return showTriangleNormals; }
    bool isShowingFaceNormals() const { return showFaceNormals; }

    void setShowTriangleNormals(bool show);
    void setShowFaceNormals(bool show);

private:
    void drawNode(aiNode* node, const aiScene* scene);
    void drawPrimitive();
    void drawTriangleNormals(); 
    void drawFaceNormals();    

    Assimp::Importer importer;
    const aiScene* scene;
    std::vector<Vertex> primitiveVertices;

    bool showTriangleNormals = false; // Variable para normales de triángulo
    bool showFaceNormals = false;     // Variable para normales de cara
};

#endif // MODELLOADER_H
