#include <cstdio>
#include "cloth.hpp"
#include "vertex.hpp"

Cloth::Cloth(glm::vec3 _position, float width, float height, int _num_particles_width, int _num_particles_height) :
        position{ _position },
        num_particles_width{ _num_particles_width },
        num_particles_height{ _num_particles_height } {
    particles.reserve(num_particles_width * num_particles_height);
    vertices.reserve(particles.capacity());
    indices.reserve((num_particles_width - 1) * (num_particles_height - 1) * 6);

    for (auto y{ 0 }; y < num_particles_height; y++) {
        for (auto x{ 0 }; x < num_particles_width; x++) {
            // create the particles in a rectangular mesh
            particles.emplace_back(glm::vec3{
                    x * width / (float) num_particles_width,
                    y * -height / (float) num_particles_height,
                    0.0f,
            } + position);

            // for each new particle, create its corresponding vertex, with no normal and some color
            vertices.push_back({ particles.back().position, {}, { x % 2 == 0, 0.0f, x % 2 != 0 }});

            // the number of faces in one direction is one less than the number of vertices in that direction
            // *-*-*-*   we have 4x3 vertices, but only 3x2 faces
            // | | | |
            // *-*-*-*
            // | | | |
            // *-*-*-*
            // so we mustn't create indices when we get at the end of a row/column
            if (x == num_particles_width - 1 || y == num_particles_height - 1) {
                continue;
            }

            // each face is composed of two triangles, so we need 6 vertices per face
            // 0   *---* 1, 4
            //     | / |
            // 2,3 *---* 5
            indices.push_back((x + 0) + (y + 0) * num_particles_width);
            indices.push_back((x + 1) + (y + 0) * num_particles_width);
            indices.push_back((x + 0) + (y + 1) * num_particles_width);
            indices.push_back((x + 0) + (y + 1) * num_particles_width);
            indices.push_back((x + 1) + (y + 0) * num_particles_width);
            indices.push_back((x + 1) + (y + 1) * num_particles_width);
        }
    }

    // create constraints with each particle's immediate neighbours
    // these represent the structural and shear constraints
    for (auto x{ 0 }; x < num_particles_width; x++) {
        for (auto y{ 0 }; y < num_particles_height; y++) {
            if (x < num_particles_width - 1) {
                Particle& p1 = get_particle(x, y);
                Particle& p2 = get_particle(x + 1, y);
                constraints.emplace_back(p1, p2);
            }
            if (y < num_particles_height - 1) {
                Particle& p1 = get_particle(x, y);
                Particle& p2 = get_particle(x, y + 1);
                constraints.emplace_back(p1, p2);
            }
            if (x < num_particles_width - 1 && y < num_particles_height - 1) {
                Particle& p1 = get_particle(x, y);
                Particle& p2 = get_particle(x + 1, y + 1);
                constraints.emplace_back(p1, p2);
            }
            if (x < num_particles_width - 1 && y < num_particles_height - 1) {
                Particle& p1 = get_particle(x + 1, y);
                Particle& p2 = get_particle(x, y + 1);
                constraints.emplace_back(p1, p2);
            }
        }
    }

    // create constraints with each particle's 2nd neighbours
    // these represent the bending constraints
    for (auto x{ 0 }; x < num_particles_width; x++) {
        for (auto y{ 0 }; y < num_particles_height; y++) {
            if (x < num_particles_width - 2) {
                Particle& p1 = get_particle(x, y);
                Particle& p2 = get_particle(x + 2, y);
                constraints.emplace_back(p1, p2);
            }
            if (y < num_particles_height - 2) {
                Particle& p1 = get_particle(x, y);
                Particle& p2 = get_particle(x, y + 2);
                constraints.emplace_back(p1, p2);
            }
            if (x < num_particles_width - 2 && y < num_particles_height - 2) {
                Particle& p1 = get_particle(x, y);
                Particle& p2 = get_particle(x + 2, y + 2);
                constraints.emplace_back(p1, p2);
            }
            if (x < num_particles_width - 2 && y < num_particles_height - 2) {
                Particle& p1 = get_particle(x + 2, y);
                Particle& p2 = get_particle(x, y + 2);
                constraints.emplace_back(p1, p2);
            }
        }
    }

    // make the upper corners immovable
    for (auto i{ 0 }; i < 3; i++) {
        get_particle(i, 0).movable                           = false;
        get_particle(num_particles_width - 1 - i, 0).movable = false;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    // bind the VBO and allocate enough data for the vertices, but don't store anything in the buffer yet
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // bind the EBO and store the computed indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));

    glBindVertexArray(0);
}

Particle& Cloth::get_particle(int x, int y) {
    return particles[y * num_particles_width + x];
}

glm::vec3 Cloth::triangle_normal(Particle& p1, Particle& p2, Particle& p3) {
    return glm::cross(p2.position - p1.position, p3.position - p1.position);
}

void Cloth::add_wind(Particle& p1, Particle& p2, Particle& p3, glm::vec3 direction) {
    auto normal{ glm::normalize(triangle_normal(p1, p2, p3)) };
    auto force{ normal * glm::dot(normal, direction) };
    p1.add_force(force);
    p2.add_force(force);
    p3.add_force(force);
}

void Cloth::draw() {
    // for each draw call we must recompute the normals, so first we reset them
    for (auto& particle : particles) {
        particle.accumulated_normal = { 0.0f, 0.f, 0.0f };
    }

    // we iterate through the faces of the mesh, and for each particle we add the normals of the triangles to which it belongs
    for (auto x{ 0 }; x < num_particles_width - 1; x++) {
        for (auto y{ 0 }; y < num_particles_height - 1; y++) {
            auto normal{ triangle_normal(get_particle(x + 1, y), get_particle(x, y), get_particle(x, y + 1)) };
            get_particle(x + 1, y).add_normal(normal);
            get_particle(x, y).add_normal(normal);
            get_particle(x, y + 1).add_normal(normal);

            normal = triangle_normal(get_particle(x + 1, y + 1), get_particle(x + 1, y), get_particle(x, y + 1));
            get_particle(x + 1, y + 1).add_normal(normal);
            get_particle(x + 1, y).add_normal(normal);
            get_particle(x, y + 1).add_normal(normal);
        }
    }

    // update the vertices based on the particles
    for (auto i{ 0 }; i < particles.size(); i++) {
        vertices[i].position = particles[i].position;
        // remember, the accumulated normal was not normalized, so we do it now
        vertices[i].normal   = glm::normalize(particles[i].accumulated_normal);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // load the vertices into the VBO
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cloth::update(const State& state) {
    for (auto i{ 0 }; i < state.constraint_iterations; i++) {
        for (auto& constraint : constraints) {
            constraint.satisfy();
        }
    }

    for (auto& particle : particles) {
        particle.update(state);
    }
}

void Cloth::add_force(glm::vec3 force) {
    for (auto& particle : particles) {
        particle.add_force(force);
    }
}

void Cloth::add_wind(glm::vec3 force) {
    for (auto x{ 0 }; x < num_particles_width - 1; x++) {
        for (int y{ 0 }; y < num_particles_height - 1; y++) {
            add_wind(get_particle(x + 1, y), get_particle(x, y), get_particle(x, y + 1), force);
            add_wind(get_particle(x + 1, y + 1), get_particle(x + 1, y), get_particle(x, y + 1), force);
        }
    }
}

void Cloth::ball_collision(const Ball& ball) {
    for (auto& particle : particles) {
        auto  v{ particle.position - ball.position };
        float l{ glm::length(v) };
        // if the particle is inside the ball
        if (l <= ball.radius) {
            // project the particle to the surface of the ball
            particle.move(glm::normalize(v) * (ball.radius - l));
        }
    }
}

