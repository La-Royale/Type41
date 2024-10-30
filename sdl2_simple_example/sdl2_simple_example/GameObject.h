#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ModelLoader.h"
#include <string>

class GameObject {
public:
    GameObject();
    ~GameObject();

    bool loadModel(const std::string& path);
    void createCube(float size);
    void createSphere(float radius, int segments, int rings);
    void draw();

private:
    ModelLoader modelLoader;
};

#endif // GAMEOBJECT_H