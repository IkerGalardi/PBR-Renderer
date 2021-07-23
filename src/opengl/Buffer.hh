#pragma once

namespace GL
{
    class Buffer
    {
    public:
        Buffer(unsigned int bufferType, void* buffer, unsigned int bufferSize, unsigned int drawType);
        Buffer(unsigned int bufferType, unsigned int drawType);
        ~Buffer();

        void Bind();
        void Unbind();

        void SetData(void* buffer, unsigned int bufferSize);
        void SetSubData(unsigned int offset, void* buffer, unsigned int bufferSize);
    private:
        unsigned int BufferID;
        unsigned int BufferType;
        unsigned int DrawType;
    protected:
    };
}