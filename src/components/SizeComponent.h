#pragma once

#include "../ecs/Component.h"

struct SizeComponent : public Component {
    float w;
    float h;
};