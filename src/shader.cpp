#include "shader.hpp"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path) : id{ glCreateProgram() } {
    auto vertex_shader_id{ compile_shader(vertex_shader_path, GL_VERTEX_SHADER) };
    auto fragment_shader_id{ compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER) };

    glAttachShader(id, vertex_shader_id);
    glAttachShader(id, fragment_shader_id);
    glLinkProgram(id);

    // the shaders can be deleted since the program has been created
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

std::string Shader::read_shader_source(const char* path) {
    std::ifstream input_file{ path, std::ios::in | std::ios::ate };
    std::string   source_code;
    source_code.reserve(input_file.tellg());
    input_file.seekg(0, std::ios::beg);
    source_code.assign(std::istreambuf_iterator<char>{ input_file }, {});
    return source_code;
}

GLuint Shader::compile_shader(const char* path, GLenum type) {
    std::string source_code{ read_shader_source(path) };
    auto        shader_id{ glCreateShader(type) };
    auto code_string{ source_code.data() };
    auto code_size{ static_cast<int>(source_code.size()) };
    glShaderSource(shader_id, 1, &code_string, &code_size);
    glCompileShader(shader_id);
    return shader_id;
}

void Shader::set(const char* name, glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set(const char* name, glm::vec4 vec) const {
    glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(vec));
}

void Shader::set(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(id, name), value);
}

