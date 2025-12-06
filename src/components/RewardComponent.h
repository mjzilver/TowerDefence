#pragma once

#include "../ecs/Component.h"

struct RewardComponent : public Component {
    int gold;
};