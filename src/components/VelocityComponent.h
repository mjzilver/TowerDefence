#pragma once

#include "../ecs/Component.h"

struct VelocityComponent : public Component {
    int x;
    int y;
};