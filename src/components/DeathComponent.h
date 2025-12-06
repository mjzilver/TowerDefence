#pragma once

#include "../ecs/Component.h"

struct DeathComponent : public Component {
    float remainingTime; 
    bool hasDied = false;
    // Cannot be made undead
    bool locked = false;
};