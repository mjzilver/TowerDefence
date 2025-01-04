#pragma once

#include "../ecs/Component.h"

struct PathfindingComponent : public Component {
    float x;
    float y;

    int currentIndex = 0;
};