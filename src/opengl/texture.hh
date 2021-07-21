#pragma once

#include <filesystem>

#include "common.hh"

namespace gl {
    class texture {
    public:
        texture();
        texture(std::filesystem::path path);
        ~texture();

        void load(std::filesystem::path path);

        void bind_to_slot(uint32 slot);
    private:
        uint32 id;
        bool initialized;
    protected:
    };
}