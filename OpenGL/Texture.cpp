#include "Texture.h"
#include "GLUtil/STB.h"
#include <string>

Texture::Texture() {
    glGenTextures(1, &id);
}

Texture::Texture(unsigned int target) : target(target) {
    glGenTextures(1, &id);
}

Texture::Texture(const char *path, unsigned int target, int desiredChannels) : path(path), target(target) {
    int nrComponents;
    data = stbi_load(path, &width, &height, &nrComponents, desiredChannels);
    if(!data)
        errors.failedToReadData = true;
    if (desiredChannels != 0)
        nrComponents = desiredChannels;
    if (nrComponents == 1) {
        format = GL_RGB;
        internalFormat = GL_R8;
    } else if (nrComponents == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    } else if (nrComponents == 4) {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }
    glGenTextures(1, &id);
    load();
    clampEdge();
    minLinear();
    magLinear();
}

Texture::Texture(const char* path, const char* ending, int desiredChannels) {
    this->target = GL_TEXTURE_CUBE_MAP;
    glGenTextures(1, &id);
    int nrComponents;
    for (int i = 0; i < 6; i++) {
        std::string newPath = path;
        switch (i) {
            case 0:
                newPath += "/right";
                break;
            case 1:
                newPath += "/left";
                break;
            case 2:
                newPath += "/top";
                break;
            case 3:
                newPath += "/bottom";
                break;
            case 4:
                newPath += "/back";
                break;
            case 5:
                newPath += "/front";
                break;
        }
        data = stbi_load(path, &width, &height, &nrComponents, desiredChannels);
        if (!data)
            errors.failedToReadData = true;
        if (desiredChannels != 0)
            nrComponents = desiredChannels;
        if (nrComponents == 1) {
            format = GL_RGB;
            internalFormat = GL_R8;
        } else if (nrComponents == 3) {
            format = GL_RGB;
            internalFormat = GL_RGB8;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
            internalFormat = GL_RGBA8;
        }
        if (!data)
            errors.failedToReadData = true;
        cubeMapData.emplace_back(data);
    }
    load();
}

void Texture::load(bool simple) {
    glBindTexture(target, id);
    if(simple){
        repeat();
        minNear();
        magNear();
        glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
    } else {
        glTexParameterf(target, GL_TEXTURE_LOD_BIAS, bias);
        repeat();
        minNearMipLinear();
        magLinear();
        glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
        generateMipMap();
    }
}

void Texture::load(){
    glBindTexture(target, id);
    switch (target){
        case GL_TEXTURE_2D:
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, fixedSamples);
            break;
        case GL_TEXTURE_2D_ARRAY:
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, depth, 0, format, type, data);
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, format, width, height, depth, fixedSamples);
            break;
        case GL_TEXTURE_3D:
            glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, format, type, data);
            break;
        case GL_TEXTURE_CUBE_MAP:
            for(int i = 0; i < 6; i++){
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, type, cubeMapData.at(i));
            }
            break;
        default:
            errors.failedToGetTextureType = true;
            break;
    }
}

void Texture::resize(int width, int height){
    this->width = width;
    this->height = height;
    glBindTexture(target, id);
    switch (target) {
        case GL_TEXTURE_2D:
            glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            glTexImage2DMultisample(target, samples, format, width, height, fixedSamples);
            break;
        case GL_TEXTURE_2D_ARRAY:
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, depth, 0, format, type, data);
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, format, width, height, depth, fixedSamples);
            break;
        case GL_TEXTURE_3D:
            glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, format, type, data);
            break;
        case GL_TEXTURE_CUBE_MAP:
            for(int i = 0; i < 6; i++){
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, type, cubeMapData.at(i));
            }
            break;
        default:
            errors.failedToGetTextureType = true;
            break;
    }
}

void Texture::bind() const {
    glBindTexture(target, id);
}

void Texture::unbind() const {
    glBindTexture(target, 0);
}

void Texture::setMaxLevel(int level) const {
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, level);
}

void Texture::setBaseLevel(int level) const {
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, level);
}

void Texture::repeat() const {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

void Texture::mirroredRepeat() const {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
}

void Texture::clampEdgeMirrored() const {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_MIRROR_CLAMP_TO_EDGE);
}

void Texture::minNear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::magNear() const {
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::minLinear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::magLinear() const {
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::minLinearMipLinear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Texture::minLinearMipNear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

void Texture::minNearMipLinear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
}

void Texture::minNearMipNear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
}

void Texture::setLodBias(float bias){
    this->bias = bias;
    glTexParameterf(target, GL_TEXTURE_LOD_BIAS, bias);
}

void Texture::clampEdge() const {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::clampBorder() const {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
}

void Texture::nearest() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::linear() const {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::generateMipMap() const {
    glGenerateMipmap(target);
}

unsigned int Texture::getID() const {
    return this->id;
}

float Texture::getTextureXOffset(int index) const{
    auto column = (index % numberOfRows);
    return factor * column;
}

float Texture::getTextureYOffset(int index) const{
    auto row = (index / numberOfRows);
    return factor * row;
}

bool Texture::hasError() const{
    return errors.failedToGetTextureType || errors.failedToReadData;
}

std::string Texture::getErrorMessage() {
    std::string result;
    if (!path.empty()) {
        result.append(path);
        result.append("\n");
    }
    if(!hasError())
        result.append("Successfully loaded");
    if(errors.failedToReadData)
        result.append("Failed to load Texture Data\n");
    if(errors.failedToGetTextureType)
        result.append("Failed to get Type\n");
    result.append("\n");
    return result;
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
    free(data);
    cubeMapData.clear();
}