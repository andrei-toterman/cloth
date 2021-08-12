#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

// represents a program linked with a vertex shader and a fragment shader
struct Shader {
    GLuint id;

    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    static GLuint compile_shader(const char* path, GLenum type);

    // convenience functions for setting uniforms

    void set(const char* name, glm::mat4 mat) const;

    void set(const char* name, glm::vec4 vec) const;
};
