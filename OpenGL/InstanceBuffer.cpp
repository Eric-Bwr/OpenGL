#include "InstanceBuffer.h"

InstanceBuffer::InstanceBuffer(const void* data, uint64_t size, unsigned int drawMode) {
    vbo = new VertexBufferObject(data, size, drawMode);
}

void InstanceBuffer::changeData(const void* data, uint64_t size, unsigned int offset, unsigned int drawMode) {
    vbo->subData(data, size, offset, drawMode);
}

InstanceBuffer::~InstanceBuffer() {
    delete vbo;
}