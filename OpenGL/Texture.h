#pragma once

#include "OpenGL.h"
#include <vector>
#include <cmath>
#include <string>

class Texture{
private:
    struct Errors {
        bool failedToLocate = false;
        bool failedToAllocate = false;
        bool failedToReadData = false;
        bool failedToGetTextureType = false;
    };
public:
    explicit Texture();
    explicit Texture(unsigned int target);
    explicit Texture(const char* path, unsigned int target = GL_TEXTURE_2D, int components = -1);
    void load(bool simple);
    void load();
    void bind() const;
    void unbind() const;
    inline void setData(uint8_t* data) { this->data = data; }
    inline void setWidth(int width) { this->width = width; }
    inline void setHeight(int height) { this->height = height; }
    inline void setDepth(unsigned int depth) { this->depth = depth; }
    inline void setSamples(unsigned int samples) { this->samples = samples; }
    inline void setFixedSamples(bool fixedSamples) { this->fixedSamples = fixedSamples; }
    inline void setInternalFormat(unsigned int internalFormat) { this->internalFormat = internalFormat; }
    inline void setFormat(unsigned int format) { this->format = format; }
    inline void setTarget(unsigned int target) { this->target = target; }
    inline void setType(unsigned int type) { this->type = type; }
    void resize(int width, int height);
    void setMaxLevel(int level) const;
    void setBaseLevel(int level) const;
    void repeat() const;
    void mirroredRepeat() const;
    void clampEdgeMirrored() const;
    void minNear() const;
    void minLinear() const;
    void minLinearMipLinear() const;
    void minLinearMipNear() const;
    void minNearMipLinear() const;
    void minNearMipNear() const;
    void setLodBias(float bias);
    void magNear() const;
    void magLinear() const;
    void clampEdge() const;
    void clampBorder() const;
    void nearest() const;
    void generateMipMap() const;
    uint8_t* getData() { return data; }
    const char* getPath() { return path; }
    unsigned int getFormat() const { return format; }
    unsigned int getInternalFormat() const { return internalFormat; }
    unsigned int getTarget() const { return target; }
    unsigned int getID() const;
    inline void setNumberOfRows(int numberOfRows) { this->numberOfRows = numberOfRows; factor = abs(1.0f / float(numberOfRows)); }
    inline int getNumberOfRows() const { return numberOfRows; }
    float getTextureXOffset(int index) const;
    float getTextureYOffset(int index) const;
    inline int getWidth() const{ return width; }
    inline int getHeight() const{ return height; }
    bool hasError() const;
    std::string getErrorMessage();
    inline Errors& getErrors(){ return errors; }
    ~Texture();
private:
    const char* path;
    bool fixedSamples = true;
    float bias = -0.4f, factor;
    unsigned int target = GL_TEXTURE_2D, internalFormat = GL_RGBA8, format = GL_RGBA, type = GL_UNSIGNED_BYTE, depth;
    unsigned int id;
    int numberOfRows = 1, samples = 4;
    int width, height;
    uint8_t* data = nullptr;
    std::vector<uint8_t*> cubeMapData;
    Errors errors;
};