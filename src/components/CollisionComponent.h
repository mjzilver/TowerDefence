#pragma once

#include "../ecs/Component.h"

struct CollisionComponent : public Component {
    // relative to the entity
    float x;
    float y;
    float w;
    float h;
    bool solid = true;
    bool colliding = false;
};