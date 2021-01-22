#pragma once

#include <GL/glew.h>

#include <vector>
#include "particle.hpp"
#include "constraint.hpp"
#include "state.hpp"
#include "ball.hpp"

struct Cloth {
    Cloth(glm::vec3 _position, float width, float height, int _num_particles_width, int _num_particles_height);

    Particle& get_particle(int x, int y);

    // computes the normal to the triangle formed by the three particles. the normal is not normalized
    static glm::vec3 triangle_normal(Particle& p1, Particle& p2, Particle& p3);

    // adds wind force to each of the three particles, proportional with the angle between the wind direction and the triangle normal
    static void add_wind(Particle& p1, Particle& p2, Particle& p3, glm::vec3 direction);

    // adds wind force to the entire cloth
    void add_wind(glm::vec3 force);

    void draw();

    // satisfy each constraint and update particles
    void update(const State& state);

    // adds a force to each particle
    void add_force(glm::vec3 force);

    // updates the particles such that the cloth does not collide with the ball
    void ball_collision(const Ball& ball);

    int num_particles_width;
    int num_particles_height;

    glm::vec3 position;

    std::vector<Particle>   particles;
    std::vector<Constraint> constraints;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint              vao{}, vbo{}, ebo{};
};
