#pragma once

#include <filesystem>

namespace gl {
    class texture {
    public:
        texture();
        texture(std::filesystem::path path);
        ~texture();

        void load(std::filesystem::path path);

        void bind_to_slot(uint32_t slot);
    private:
        uint32_t id;
        bool initialized;
    protected:
    };
}