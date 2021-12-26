#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::attachColorBuffer(Texture& colorBuffer, unsigned int attachment){
    colorBuffer.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, colorBuffer.getTarget(), colorBuffer.getID(), 0);
    colorBuffers.emplace_back(&colorBuffer);
}

void FrameBuffer::attachRenderBuffer(RenderBuffer& renderBuffer, unsigned int attachment){
    renderBuffer.bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer.getID());
    renderBuffers.emplace_back(&renderBuffer);
}

void FrameBuffer::resize(int width, int height){
    for(auto& colorBuffer : colorBuffers)
        colorBuffer->resize(width, height);
    for(auto& renderBuffer : renderBuffers)
        renderBuffer->resize(width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

int FrameBuffer::finish(){
    int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return result;
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::bindRead() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::bindDraw() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::getID() const{
    return frameBuffer;
}

FrameBuffer::~FrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glDeleteFramebuffers(1, &frameBuffer);
    colorBuffers.clear();
    renderBuffers.clear();
}