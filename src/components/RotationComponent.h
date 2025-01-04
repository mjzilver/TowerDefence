#pragma once

#include "../ecs/Component.h"

struct RotationComponent : public Component {
    // 0 - 360
    float angle;
};