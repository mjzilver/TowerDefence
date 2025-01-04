#pragma once

#include "../ecs/Component.h"

struct DamageComponent : public Component {
    int damage;
};