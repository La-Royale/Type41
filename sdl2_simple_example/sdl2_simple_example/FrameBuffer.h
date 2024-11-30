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
    int width() const { return _width; }
    int height() const { return _height; }

private:
    GLuint framebuffer;
    GLuint texture;
    GLuint depthRenderbuffer;
    int _width;
    int _height;
};
