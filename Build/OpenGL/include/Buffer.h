#pragma once

#include "OpenGL.h"
#include <vector>

struct VertexElement {
    unsigned int type;
    unsigned int count;
    bool normalized;
    bool divided;
    static unsigned int getSizeOfType(unsigned int type){
        switch(type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};

class VertexBufferObjectLayout {
public:
    VertexBufferObjectLayout();
    void pushFloat(unsigned int count);
    void pushFloatDivided(unsigned int count);
    void pushInt(unsigned int count);
    void pushIntDivided(unsigned int count);
    void pushByte(unsigned int count);
    void pushByteDivided(unsigned int count);
    inline const std::vector<VertexElement>& getElements() const { return elements; }
    inline unsigned int getStride() const { return stride; }
    ~VertexBufferObjectLayout();
private:
    std::vector<VertexElement> elements;
    unsigned int stride;
};

class VertexBufferObject {
public:
    VertexBufferObject();
    VertexBufferObject(const void* data, uint64_t size, unsigned int drawMode);
    void init(const void* data, uint64_t size, unsigned int drawMode);
    void subData(const void* data, uint64_t size, unsigned int offset, unsigned int drawMode) const;
    void bind() const;
    static void unbind() ;
    ~VertexBufferObject();
private:
    unsigned int vbo;
};

class VertexArrayObject {
public:
    VertexArrayObject();
    void addBuffer(const VertexBufferObject& vbo, const VertexBufferObjectLayout& vertexArrayObjectLayout) const;
    void bind() const;
    static void unbind() ;
    inline unsigned int getVertexArray() const { return vao; }
    ~VertexArrayObject();
private:
    unsigned int vao;
};

class IndicesBufferObject {
public:
    IndicesBufferObject(const unsigned int* data, uint64_t count, unsigned int drawType);
    void subData(const void* data, uint64_t size, unsigned int offset, unsigned int drawMode) const;
    void bind() const;
    static void unbind();
    inline unsigned int getCount() const { return innerCount; }
    ~IndicesBufferObject();
private:
    uint64_t innerCount;
    unsigned int ibo;
};