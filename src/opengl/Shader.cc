#include "Shader.hh"

#include <fstream>
#include <iostream> 
#include <sstream>
#include <cstdio>

#include <GL/glew.h>
#include <spdlog/spdlog.h>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#define PRINT(X) std::cout << X << std::endl

namespace GL 
{
    Shader::Shader(const std::string& vs, const std::string& fs) 
    {
        // Create the vertex shader
        const char* vertexSource = vs.c_str();
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);
        PrintShaderErrors(GL_VERTEX_SHADER, vertexShader);
        
        // Create the fragment shader
        const char* fragmentSource = fs.c_str();
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);
        PrintShaderErrors(GL_FRAGMENT_SHADER, fragmentShader);

        // Build the shader program
        ProgramID = glCreateProgram();
        glAttachShader(ProgramID, vertexShader);
        glAttachShader(ProgramID, fragmentShader);
        glLinkProgram(ProgramID);
        glValidateProgram(ProgramID);

        // Cleanup of shaders (program will still work)
        glDetachShader(ProgramID, vertexShader);
        glDeleteShader(vertexShader);
        glDetachShader(ProgramID, fragmentShader);
        glDeleteShader(fragmentShader);

        spdlog::trace("OpenGL: created shader with id {}", ProgramID);
    }

    Shader::~Shader() 
    {
        glDeleteProgram(ProgramID);
    }

    void Shader::Bind()
    {
        glUseProgram(ProgramID);
    }
    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    std::shared_ptr<Shader> Shader::FromFile(const std::string& shader)
    {
        enum class ShaderType
        {
            Vertex = 0,
            Fragment = 1,
            None = -1
        } currentShaderType;

        std::ifstream shaderFile(shader);
        std::stringstream ss[2];

        std::string temp;
        while(std::getline(shaderFile, temp))
        {
            if(temp.find("#shader frag") != std::string::npos)
                currentShaderType = ShaderType::Fragment;
            else if(temp.find("#shader vert") != std::string::npos)
                currentShaderType = ShaderType::Vertex;
            else 
                ss[(size_t)currentShaderType] << temp << "\n";
        }

        std::string vertexSource = ss[(size_t)ShaderType::Vertex].str().append("\0");
        std::string fragmentSource = ss[(size_t)ShaderType::Fragment].str().append("\0");

        return std::make_shared<Shader>(vertexSource, fragmentSource);
    }
    
    void Shader::SetUniformVector(const char* name, const glm::vec2& vector)
    {
        int loc = glGetUniformLocation(ProgramID, name);
        glUniform2f(loc, vector.x, vector.y);
    }

    void Shader::SetUniformVector(const char* name, const glm::vec3& vector)
    {
        int loc = glGetUniformLocation(ProgramID, name);
        glUniform3f(loc, vector.x, vector.y, vector.z);
    }
    
    void Shader::SetUniformMatrix(const char* name, const glm::mat4& matrix)
    {
        int loc = glGetUniformLocation(ProgramID, name);
        if(loc == -1)
            spdlog::warn("OpenGL: uniform {} could not be found", name);

        spdlog::trace("OpenGL: {} \n {}", name, glm::to_string(matrix));

        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void Shader::SetUniformTexture(const char* name, unsigned int textureSlot)
    {
        int loc = glGetUniformLocation(ProgramID, name);
        glUniform1i(loc, textureSlot);
    }

    void Shader::PrintShaderErrors(unsigned int shaderType, int shaderID)
    {
        int success;
        char log[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(shaderID, 512, NULL, log);
            if(shaderType == GL_VERTEX_SHADER)
                std::cout << "shader_type -> VERTEX_SHADER\n";
            else if(shaderType == GL_FRAGMENT_SHADER)
                std::cout << "shader_type -> FRAGMENT_SHADER\n";
            else if(shaderType == GL_PROGRAM)
                std::cout << "shader_type -> SHADER_PROGRAM\n";

            std::cout << log << std::endl;
        } 
        else 
        {
            spdlog::trace("OpenGL: shader compilation went alright");
        }
    }
}
