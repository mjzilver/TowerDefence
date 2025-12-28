#pragma once

#include "../ecs/System.h"
#include "../event/Event.h"

class CombatSystem : public System {
public:
    CombatSystem(EngineContext& ctx);

    void update(float) override {}

    void onEvent(const Event& event);

    void reset() override {};
};
