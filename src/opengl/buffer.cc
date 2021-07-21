#include "buffer.hh"

namespace gl {
        buffer::buffer(buffer_type btype, draw_type dtype)
            : btype(btype), 
              dtype(dtype)
        {
            glGenBuffers(1, &id);
        }

        buffer::~buffer() {
            glDeleteBuffers(1, &id);
        }

        void buffer::set_data(const void* data, uint32 size) {
            glBindBuffer(static_cast<uint32>(btype), id);
            glBufferData(static_cast<uint32>(btype), size, data, static_cast<uint32>(dtype));
        }
}