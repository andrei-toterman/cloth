#include "constraint.hpp"

Constraint::Constraint(Particle& _p1, Particle& _p2) :
        p1{ _p1 }, p2{ _p2 }, rest_distance{ glm::length(_p1.position - _p2.position) } {
}

void Constraint::satisfy() const {
    auto p1_to_p2{ p2.position - p1.position };
    auto current_distance{ glm::length(p1_to_p2) };
    // the offset vector that could move p1 into a distance of rest_distance to p2
    auto correction{ p1_to_p2 * (1 - rest_distance / current_distance) };
    // move both particles along the correction vector, in opposite directions, at half its length aka symmetric correction
    p1.move(correction * 0.5f);
    p2.move(correction * -0.5f);
}
