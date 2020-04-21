#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(unsigned int format, int width, int height)
: format(format), multisampled(false){
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, format , width, height);
}

RenderBuffer::RenderBuffer(int samples, unsigned int format, int width, int height)
: format(format), multisampled(true), samples(samples) {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
}

void RenderBuffer::resize(int width, int height){
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    if(multisampled)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, format , width, height);
}

void RenderBuffer::bind(){
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::unbind(){
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned int RenderBuffer::getID(){
    return id;
}

RenderBuffer::~RenderBuffer(){
    glDeleteRenderbuffers(1, &id);
}