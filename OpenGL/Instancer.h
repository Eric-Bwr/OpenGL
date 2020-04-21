#pragma once

#include "InstanceBuffer.h"

class Instancer {
public:
    Instancer();
    void addBuffer(InstanceBuffer& buffer, VertexBufferObjectLayout& layout);
    void bind();
    void unbind();
    ~Instancer();
private:
    VertexArrayObject* vao;
    std::vector<InstanceBuffer*> buffers;
};