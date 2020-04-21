#pragma once

#include "Texture.h"
#include "RenderBuffer.h"

class FrameBuffer {
public:
    FrameBuffer();
    void attachColorBuffer(Texture& colorBuffer, unsigned int attachment);
    void attachRenderBuffer(RenderBuffer& renderBuffer, unsigned int attachment);
    void resize(int width, int height);
    static int finish();
    void bind() const;
    void bindRead() const;
    void bindDraw() const;
    static void unbind();
    unsigned int getID() const;
    ~FrameBuffer();
private:
    unsigned int frameBuffer;
    std::vector<Texture*> colorBuffers;
    std::vector<RenderBuffer*> renderBuffers;
};