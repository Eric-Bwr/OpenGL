#pragma once

#include "OpenGL.h"

class RenderBuffer {
public:
    RenderBuffer(unsigned int format, int width, int height);
    RenderBuffer(int samples, unsigned int format, int width, int height);
    void resize(int width, int height);
    void bind();
    void unbind();
    unsigned int getID();
    ~RenderBuffer();
private:
    unsigned int id;
    unsigned int format;
    int samples;
    bool multisampled;
};