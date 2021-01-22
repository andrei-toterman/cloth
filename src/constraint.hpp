#pragma once

#include "particle.hpp"

// this represents the constraint that must be satisfied between two particles in order to represent a cloth
struct Constraint {
    Constraint(Particle& _p1, Particle& _p2);

    void satisfy() const;

    // the distance between the particles at which they are "at rest" aka the constraint is satisfied
    // it is initialized with the initial distance between the particles
    float rest_distance;
    Particle& p1;
    Particle& p2;
};
