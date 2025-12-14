#include "Color.h"

#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <random>

constexpr std::array<glm::vec3, 20> UI_COLORS = {{SOFT_BLUE,   MUTED_CYAN,   SOFT_GREEN, WARM_SAND,    SOFT_RED, MUTED_PURPLE, LAVENDER_BLUE,
                                                  TEAL,        SOFT_ORANGE,  OLIVE,      NEUTRAL_GRAY, ROSE,     SKY_BLUE,     MINT,
                                                  SOFT_YELLOW, COFFEE_BROWN, BLUSH,      SLATE_BLUE,   LILAC,    MUTED_FOREST}};

std::array<glm::vec3, 20> shuffledColors = [] {
    std::array<glm::vec3, 20> arr = UI_COLORS;
    std::mt19937 rng(0x3FCCB);
    std::shuffle(arr.begin(), arr.end(), rng);
    return arr;
}();

glm::vec3 getColor(int i) { return shuffledColors[i % shuffledColors.size()]; }

glm::vec3 getColor() {
    static int i = 0;
    return getColor(i++);
}
