#include "Instancer.h"

Instancer::Instancer() {
    vao = new VertexArrayObject();
}

void Instancer::addBuffer(InstanceBuffer& buffer, VertexBufferObjectLayout& layout) {
    vao->addBuffer(buffer.getBuffer(), layout);
}

void Instancer::bind() {
    vao->bind();
}

void Instancer::unbind() {
    vao->unbind();
}

Instancer::~Instancer() {
    delete vao;
    buffers.clear();
}