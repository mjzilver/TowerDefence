#pragma once

#include "../ecs/Component.h"
#include "../utils/Direction.h"

enum FlagType {
    Start,
    End,
};

struct FlagComponent : public Component {
    FlagType type;
};