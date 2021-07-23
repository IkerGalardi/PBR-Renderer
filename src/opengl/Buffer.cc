#include "Buffer.hh"

#include <GL/glew.h>

namespace GL
{
    Buffer::Buffer(unsigned int bufferType, void* buffer, unsigned int bufferSize, unsigned int drawType)
        : 
        BufferType(bufferType),
        DrawType(drawType)
    {
        glGenBuffers(1, &BufferID);
        SetData(buffer, bufferSize);
    }

    Buffer::Buffer(unsigned int bufferType, unsigned int drawType)
        : 
        BufferType(bufferType),
        DrawType(drawType)
    {
        glGenBuffers(1, &BufferID);
    }
    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &BufferID);
    }

    void Buffer::Bind()
    {
        glBindBuffer(BufferType, BufferID);
    }
    void Buffer::Unbind()
    {
        glBindBuffer(BufferType, 0);
    }
    void Buffer::SetData(void* buffer, unsigned int bufferSize)
    {
        Bind();
        glBufferData(BufferType, bufferSize, buffer, DrawType);
    }
    void Buffer::SetSubData(unsigned int offset, void* buffer, unsigned int bufferSize)
    {
        Bind();
        glBufferSubData(BufferType, offset, bufferSize, buffer);
    }
}