#include "Buffer.h"

VertexBufferObjectLayout::VertexBufferObjectLayout() : stride(0) {}

void VertexBufferObjectLayout::pushFloat(unsigned int count) {
    elements.emplace_back(VertexElement{GL_FLOAT, count, GL_FALSE, false});
    stride += count * VertexElement::getSizeOfType(GL_FLOAT);
}

void VertexBufferObjectLayout::pushFloatDivided(unsigned int count) {
    elements.emplace_back(VertexElement{GL_FLOAT, count, GL_FALSE, true});
    stride += count * VertexElement::getSizeOfType(GL_FLOAT);
}

void VertexBufferObjectLayout::pushDouble(unsigned int count) {
    elements.emplace_back(VertexElement{GL_DOUBLE, count, GL_FALSE, false});
    stride += count * VertexElement::getSizeOfType(GL_DOUBLE);
}

void VertexBufferObjectLayout::pushDoubleDivided(unsigned int count) {
    elements.emplace_back(VertexElement{GL_DOUBLE, count, GL_FALSE, true});
    stride += count * VertexElement::getSizeOfType(GL_DOUBLE);
}

void VertexBufferObjectLayout::pushInt(unsigned int count) {
    elements.emplace_back(VertexElement{GL_UNSIGNED_INT, count, GL_FALSE, false});
    stride += count * VertexElement::getSizeOfType(GL_UNSIGNED_INT);
}

void VertexBufferObjectLayout::pushIntDivided(unsigned int count) {
    elements.emplace_back(VertexElement{GL_UNSIGNED_INT, count, GL_FALSE, true});
    stride += count * VertexElement::getSizeOfType(GL_UNSIGNED_INT);
}

void VertexBufferObjectLayout::pushByte(unsigned int count) {
    elements.emplace_back(VertexElement{GL_UNSIGNED_BYTE, count, GL_TRUE, false});
    stride += count * VertexElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

void VertexBufferObjectLayout::pushByteDivided(unsigned int count) {
    elements.emplace_back(VertexElement{GL_UNSIGNED_BYTE, count, GL_TRUE, true});
    stride += count * VertexElement::getSizeOfType(GL_UNSIGNED_BYTE);
}

VertexBufferObjectLayout::~VertexBufferObjectLayout() {
    elements.clear();
}

VertexBufferObject::VertexBufferObject() = default;

VertexBufferObject::VertexBufferObject(const void *data, uint64_t size, unsigned int drawMode) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
}

void VertexBufferObject::init(const void *data, uint64_t size, unsigned int drawMode) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
}

void VertexBufferObject::subData(const void *data, uint64_t size, unsigned int offset, unsigned int drawMode) const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, drawMode);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBufferObject::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBufferObject::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::~VertexBufferObject() {
    unbind();
    glDeleteBuffers(1, &vbo);
}

VertexArrayObject::VertexArrayObject() = default;

void VertexArrayObject::init() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void VertexArrayObject::addBuffer(const VertexBufferObject &vbo, const VertexBufferObjectLayout &vertexArrayObjectLayout) const {
    bind();
    vbo.bind();
    const auto &elements = vertexArrayObjectLayout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto &element = elements.at(i);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, vertexArrayObjectLayout.getStride(), (const void *) offset);
        offset += element.count * VertexElement::getSizeOfType(element.type);
        if (element.divided)
            glVertexAttribDivisor(i, 1);
    }
}

void VertexArrayObject::bind() const {
    glBindVertexArray(vao);
}

void VertexArrayObject::unbind() {
    glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject() {
    unbind();
    glDeleteVertexArrays(1, &vao);
}

IndicesBufferObject::IndicesBufferObject() = default;

IndicesBufferObject::IndicesBufferObject(const unsigned int *data, uint64_t count, unsigned int drawType) : count(count) {
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawType);
}

void IndicesBufferObject::init(const unsigned int *data, uint64_t count, unsigned int drawType) {
    this->count = count;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawType);
}

void IndicesBufferObject::subData(const void *data, uint64_t size, unsigned int offset, unsigned int drawMode) const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, drawMode);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void IndicesBufferObject::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void IndicesBufferObject::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndicesBufferObject::~IndicesBufferObject() {
    unbind();
    glDeleteBuffers(1, &ibo);
}