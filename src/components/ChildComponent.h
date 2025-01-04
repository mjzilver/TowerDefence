#pragma once

#include "../ecs/Component.h"

struct ChildComponent : public Component {
    Entity child;
};