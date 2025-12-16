#pragma once

#include "../ecs/System.h"

class ClickSystem : public System {
public:
    ClickSystem(EngineContext& ctx);

    void update(float) override {}

    void onClick(int button, int action, double x, double y);

    void onHover(double x, double y);

    void reset() override {};
};