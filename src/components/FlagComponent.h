#pragma once

#include "../ecs/Component.h"
#include "../utils/Direction.h"

enum FlagType {
    Start,
    End,
    Path,
};

struct FlagComponent : public Component {
    FlagType type;
};