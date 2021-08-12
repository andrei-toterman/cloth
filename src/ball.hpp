#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "vertex.hpp"
#include "state.hpp"

// a ball that will interact with the cloth
struct Ball {
    Ball(glm::vec3 _position, float _radius, glm::vec3 color);

    void update(const State& state);

    void draw() const;

    glm::vec3 position;
    float     radius;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint              vao{}, vbo{}, ebo{};

    // sphere vertex resolution
    static constexpr auto resolution = 48;
    // some arbitrary velocity
    static constexpr auto velocity   = 0.25f;
};
