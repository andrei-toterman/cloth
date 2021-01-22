#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "vertex.hpp"
#include "state.hpp"

// a ball that will interact with the cloth
struct Ball {
    Ball(glm::vec3 _position, glm::vec3 _color, float _radius);

    void update(const State& state);

    void draw() const;

    glm::vec3 position;
    float     radius;
    glm::vec3 color;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint              vao{}, vbo{}, ebo{};
};
