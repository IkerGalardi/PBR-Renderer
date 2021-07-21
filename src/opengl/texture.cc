#include "texture.hh"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <GL/glew.h>

namespace gl {
    texture::texture() 
        : initialized(false),
          id(0)
    {
    }

    texture::texture(std::filesystem::path path) {
        load(path);
    }

    texture::~texture() {
        glDeleteTextures(1, &id);
    }

    void texture::load(std::filesystem::path path) {
        // Load the image. SOIL is being used right now because it was thought at the 
        // start that all the texture things would be done through that library. Right
        // now is only used as a replacement of stb_image
        int32 width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        uint8* image = stbi_load(path.c_str(), &width, &height, &channels, 4);
        if(image == nullptr) {
            std::cerr << "No texture found" << std::endl;
        }

        // Create the texture and bind it
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // Setup the settings of the texture:
        //  - Min and mag filters are set to nearest as the game will be using 
        //    pixel art, and interpolation would break that pixel art
        //  - Wrapping is set to clamping with no reasoning behind
        /// TODO: expose as parameters to the constructor?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Upload the data to the gpu and generate the mipmaps for completeness
        // sake
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, id);

        initialized = true;
    }

    void texture::bind_to_slot(uint32 slot) {
        //std::cout << "Bound texture " << id << " to slot " << slot << std::endl;

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }
}