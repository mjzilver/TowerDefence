#pragma once

#include "../ecs/System.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class CombatSystem : public System {
public:
    CombatSystem(EngineContext& ctx) : System(ctx) {
        context.eventDispatcher.addListener(EventType::PROJECTILE_HIT, std::bind(&CombatSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float) override {}

    void onEvent(const Event& event);

    void reset() override {};
};
