#pragma once

#include "Buffer.h"

class InstanceBuffer {
public:
    explicit InstanceBuffer(const void* data, uint64_t size, unsigned int drawMode);
    void changeData(const void* data, uint64_t size, unsigned int offset, unsigned int drawMode);
    inline VertexBufferObject& getBuffer() { return *vbo; }
    ~InstanceBuffer();
private:
    VertexBufferObject* vbo;
};