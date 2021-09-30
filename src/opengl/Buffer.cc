#include "Buffer.hh"

#include <GL/glew.h>

namespace GL
{
    Buffer::Buffer(unsigned int bufferType, void* buffer, unsigned int bufferSize, unsigned int drawType)
        : 
        BufferType(bufferType),
        DrawType(drawType),
        BufferSize(bufferSize)
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

    Buffer::Buffer(const Buffer& other) {
        // Prepare the buffers
        glBindBuffer(GL_COPY_READ_BUFFER, BufferID);
        glBufferData(GL_COPY_READ_BUFFER, 0, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_COPY_WRITE_BUFFER, other.BufferID);

        // Transfer the data from one buffer to the other
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, BufferSize);
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
    void Buffer::SetData(const void* buffer, unsigned int bufferSize)
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