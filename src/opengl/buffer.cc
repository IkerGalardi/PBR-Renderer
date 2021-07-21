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

        void buffer::set_data(const void* data, uint32_t size) {
            glBindBuffer(static_cast<uint32_t>(btype), id);
            glBufferData(static_cast<uint32_t>(btype), size, data, static_cast<uint32_t>(dtype));
        }
}