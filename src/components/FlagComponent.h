#pragma once

#include "../ecs/Component.h"

enum FlagType {
    START,
    END,
    PATH,
};

struct FlagComponent : public Component {
    FlagType type;
};