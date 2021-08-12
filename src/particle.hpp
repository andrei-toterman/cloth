#pragma once

#include <glm/glm.hpp>
#include "state.hpp"

// a particle that will be constrained with other particles to form a mesh representing a cloth
struct Particle {
    explicit Particle(glm::vec3 _position);

    void update();

    void move(glm::vec3 offset);

    void add_force(glm::vec3 force);

    // adds a normal vector to the accumulated normal
    void add_normal(glm::vec3 normal);

    bool      movable;
    glm::vec3 position;
    // old position is needed when computing the new position based on acceleration
    glm::vec3 old_position;
    glm::vec3 acceleration;
    // the normal of the particle, which will be computed using its neighbours' normals. it will be normalized only when needed
    glm::vec3 accumulated_normal;

    // used in Verlet integration
    static constexpr float damping         = 0.01f;
    static constexpr float time_step_size2 = 0.5f * 0.5f;
};
