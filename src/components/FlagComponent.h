#pragma once

#include "../ecs/Component.h"
#include "../utils/Direction.h"

enum FlagType {
    START,
    END,
    PATH,
};

struct FlagComponent : public Component {
    FlagType type;
};