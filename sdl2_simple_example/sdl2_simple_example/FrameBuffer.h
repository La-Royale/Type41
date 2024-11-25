#pragma once
#include <GL/glew.h>

class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    void Init(int width, int height);
    void Resize(int width, int height);
    void Bind();
    void Unbind();

    GLuint GetTexture() const;

private:
    GLuint framebuffer;
    GLuint texture;
    GLuint depthRenderbuffer;
};
