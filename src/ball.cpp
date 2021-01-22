#include "ball.hpp"

#include <glm/gtc/constants.hpp>

Ball::Ball(glm::vec3 _position, glm::vec3 _color, float _radius) :
        position{ _position }, radius{ _radius }, color{ _color } {
    const auto precision{ 48 };
    vertices.reserve((precision + 1) * (precision + 1));
    indices.reserve(precision * precision * 6);

    // create a ball using spherical coordinates
    for (auto i{ 0 }; i <= precision; i++) {
        auto      phi{ glm::pi<float>() * i / precision };
        for (auto j{ 0 }; j <= precision; j++) {
            auto theta{ glm::pi<float>() * 2.0f * j / precision };
            auto x{ -glm::cos(theta) * glm::sin(phi) };
            auto y{ glm::cos(phi) };
            auto z{ glm::sin(theta) * glm::sin(phi) };
            // make the actual radius of the vertices a little lower, it interacts a little nicer with the cloth
            vertices.push_back({ radius * 0.8f * glm::vec3{ x, y, z }, { x, y, z }, color });

            // see cloth explanation
            if (i == precision || j == precision) {
                continue;
            }

            indices.push_back(i * (precision + 1) + j);
            indices.push_back(i * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j);
            indices.push_back(i * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j);
        }
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

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

void Ball::update(const State& state) {
    // technically this doesn't make sense but it's a good enough velocity for the ball
    auto      velocity{ state.time_step_size2 };
    // direction vectors
    glm::vec3 front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 right{ 1.0f, 0.0f, 0.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    // check which keys are pressed and update the ball position accordingly
    // each key is a bool, so it can be converted to 1 or 0, so if the key is not pressed, it's basically nullifying the move
    position += (float) state.keys[State::w] * velocity * front;
    position -= (float) state.keys[State::a] * velocity * right;
    position -= (float) state.keys[State::s] * velocity * front;
    position += (float) state.keys[State::d] * velocity * right;
    position += (float) state.keys[State::q] * velocity * up;
    position -= (float) state.keys[State::e] * velocity * up;
}

void Ball::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
