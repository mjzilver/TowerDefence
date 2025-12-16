#pragma once

#include "../ecs/System.h"
#include "../event/Event.h"

class AnimationSystem : public System {
public:
    AnimationSystem(EngineContext& ctx);

    void update(float deltaTime) override;

    void onEvent(const Event& event);

    void reset() override { return; };
};