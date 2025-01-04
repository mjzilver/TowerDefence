#pragma once

#include "../ecs/Component.h"

struct WeaponComponent : public Component {
    int damage;
    int range;

    float projectileSpeed = 100.0f;

    // shots per second
    float rateOfFire = 1.0f;
    float timeSinceLastShot = 0.0f;
};