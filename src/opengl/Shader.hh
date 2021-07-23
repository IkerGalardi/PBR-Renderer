#pragma once

#include <string>
#include <memory>

#include "Texture.hh"
#include "glm/glm.hpp"

namespace GL
{
    class Shader
    {
        friend class Texture;
    public:
        Shader() = default;
        Shader(const std::string& vs, const std::string& fs);
        ~Shader();

        void Bind();
        void Unbind();

        static std::shared_ptr<Shader> FromFile(const std::string& shader);
        
        void SetUniformTexture(const char* name, unsigned int textureSlot);
        void SetUniformVector(const char* name, const glm::vec2& vector);
        void SetUniformVector(const char* name, const glm::vec3& vector);
        void SetUniformMatrix(const char* name, const glm::mat4& matrix);
    private:
        void PrintShaderErrors(unsigned int shaderType, int shaderID);

        unsigned int ProgramID;
    protected:
    };
}