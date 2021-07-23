#include "renderer.hh"

#include <vector>

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include <GL/glew.h>

#include "opengl/OpenGL.hh"

SDL_Window* window_handle;
SDL_GLContext opengl_context;

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
            spdlog::info("OpenGL: {}", message);
            break;

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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);        

        // Create the OpenGL context and make it current
        opengl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, opengl_context);

        spdlog::info("Renderer: OpenGL context created");

        // Disable VSync
        SDL_GL_SetSwapInterval(0);

        // Initialize glew, don't ask why use glew experimental
        glewExperimental = true;
        auto init_status = glewInit();
        if(init_status != GLEW_OK) {
            spdlog::error("OpenGL: glew failed with error code {} on initialization", init_status);
            std::exit(2);
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

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

        // Save the window handle for future use (framebuffer swaping)
        window_handle = window;
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window_handle);
    }

    void destroy() {
    }
}