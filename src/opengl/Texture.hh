#pragma once

namespace GL
{
    class Texture
    {
    friend class Framebuffer;
    public:
        Texture(const char* filepath);
        Texture(unsigned int width, unsigned int height);
        ~Texture();

        void Bind(unsigned int textureSlot = 0);
    private:
        unsigned int TextureID;
    protected:
    };
}