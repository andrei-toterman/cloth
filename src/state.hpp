#pragma once

#include <array>

struct State {
    float        damping{ 0.01f };
    float        time_step_size2{ 0.5f * 0.5f };
    unsigned int constraint_iterations{ 30 };

    enum {
        q, e, a, d, w, s, space, keys_n
    };

    std::array<bool, keys_n> keys{};
};
