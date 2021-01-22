#include "particle.hpp"

Particle::Particle(glm::vec3 _position) :
        position{ _position },
        old_position{ _position },
        acceleration{ 0.0f, 0.0f, 0.0f },
        movable{ true },
        accumulated_normal{ 0.0f, 0.0f, 0.0f } {
}

void Particle::add_force(glm::vec3 force) {
    acceleration += force;
}

void Particle::update(const State& state) {
    if (movable) {
        auto temp{ position };
        // compute the new position by integrating the acceleration
        position += (position - old_position) * (1.0f - state.damping) + acceleration * state.time_step_size2;
        old_position = temp;
        // acceleration has been transformed into position, so it is nullified
        acceleration = { 0.0f, 0.0f, 0.0f };
    }
}

void Particle::move(const glm::vec3 offset) {
    if (movable) {
        position += offset;
    }
}

void Particle::add_normal(glm::vec3 normal) {
    accumulated_normal += glm::normalize(normal);
}


