#pragma once

#include "../ecs/Component.h"
#include "../utils/ZLayer.h"

struct PositionComponent : public Component {
    float x;
    float y;
    ZLayer zIndex = ZLayer::GROUND;
};