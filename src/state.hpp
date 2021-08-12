#pragma once

#include <array>

// I don't want an enum class since I will use those as indices, so a namespace will do the job
namespace Keys {
    enum {
        q, e, a, d, w, s, space, keys_n
    };
}

struct State {
    std::array<bool, Keys::keys_n> keys{};
};
