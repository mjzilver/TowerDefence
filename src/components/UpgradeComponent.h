#pragma once

#include "../ecs/Component.h"

struct UpgradeComponent : public Component {
    int upgradeLevel = 1;
    int maxUpgradeLevel = 1;
};