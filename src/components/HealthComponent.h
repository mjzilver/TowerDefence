#pragma once

#include "../ecs/Component.h"

struct HealthComponent : public Component {
    int health = 1;
};