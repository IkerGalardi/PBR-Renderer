#include "renderer.hh"

#include <vector>
#include <cstdio>

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "opengl/OpenGL.hh"

SDL_Window* window_handle;
SDL_GLContext opengl_context;

std::shared_ptr<GL::Shader> shader;

static std::string read_file(const std::filesystem::path& path) {
    FILE* file = fopen(path.c_str(), "r");
    fseek(file, 0, SEEK_END);

    int size = ftell(file);
    char* file_contets = new char[size + 1];

    fseek(file, 0, SEEK_SET);
    fread(file_contets, sizeof(char), size, file);

    std::string result{file_contets};
    
    delete file_contets;

    return result;
}

static void on_opengl_error(GLenum source, GLenum type, GLuint id, GLenum severity, 
                            GLsizei length, const GLchar* message, const void* userPara) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            spdlog::error("OpenGL: {}", message);
            std::exit(2);
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            spdlog::warn("OpenGL: {}", message);
            break;

        case GL_DEBUG_SEVERITY_LOW:
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            spdlog::trace("OpenGL: {}", message);
            break;
    }
}

namespace renderer {
    void initialize(SDL_Window* window) {
        // Setup viewport framebuffer settings
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        // Create the OpenGL context and make it current
        opengl_context = SDL_GL_CreateContext(window);
        if(opengl_context == NULL) {
            spdlog::error("SDL: error while creating the OpenGL context: {}", SDL_GetError());
            std::exit(1);
        }
        auto makecurrent_err = SDL_GL_MakeCurrent(window, opengl_context);
        if(makecurrent_err != 0) {
            spdlog::error("SDL: error while making the context current: {}", SDL_GetError());
            std::exit(1);
        }

        spdlog::info("Renderer: OpenGL context created");

        // Disable VSync
        SDL_GL_SetSwapInterval(0);

        // Initialize glew, don't ask why use glew experimental
        glewExperimental = false;
        auto init_status = glewInit();
        if(init_status != GLEW_OK) {
            if(init_status > 3) {
                spdlog::warn("OpenGL: ingoring glew initialization strange failure ({}: {})", 
                             init_status, 
                             glewGetErrorString(init_status));
            }else {
                spdlog::error("OpenGL: glew failed with error code {}: {}", init_status, glewGetErrorString(init_status));
                std::exit(2);
            }


        }

        spdlog::info("Renderer: OpenGL initialized");

        // Setup the opengl viewport size to the window size
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Enable debug messaging with custom callback
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(on_opengl_error, nullptr);

        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Save the window handle for future use (framebuffer swaping and aspect ratio calculation)
        window_handle = window;

        shader = GL::Shader::FromFile("shaders/pbr.glsl");
    }

    void begin(const scene_data& scene_data) {
        // Clear the framebuffer to redraw on it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Bind();

        // Calculate the projection matrix
        int width, height;
        SDL_GetWindowSize(window_handle, &width, &height);
        float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
        glm::mat4 projection_matrix = glm::perspective(glm::radians(scene_data.camera_fov), aspect_ratio, 0.01f, 100.0f);
        shader->SetUniformMatrix("u_proj", projection_matrix);

        // Calculate the view matrix
        glm::mat4 view_matrix{1.0f};
        view_matrix = glm::translate(view_matrix, -scene_data.camera_position);
        shader->SetUniformMatrix("u_view", view_matrix);

        // Send the camera position
        shader->SetUniformVector("u_camera_position", scene_data.camera_position);
    }

    void render(const model& model) {
        model.mesh_data->vertex_array->Bind();

        // Calculate the model transformation matrix
        glm::mat4 model_matrix{1.0f};
        model_matrix = glm::translate(model_matrix, model.position);
        shader->SetUniformMatrix("u_model", model_matrix);

        model.diffuse->Bind(0);
        model.roughness->Bind(1);
        model.normal->Bind(2);
        shader->SetUniformTexture("diffuse_texture", 0);
        shader->SetUniformTexture("roughness_texture", 1);
        shader->SetUniformTexture("normal_texture", 2);

        glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, nullptr);
    }

    void end() {
        SDL_GL_SwapWindow(window_handle);
    }

    void destroy() {
    }
}