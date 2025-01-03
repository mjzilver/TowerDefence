#pragma once

#include "../ecs/Component.h"
#include "../utils/Direction.h"

struct DirectionComponent : public Component {
    Direction direction;
};