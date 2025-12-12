#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

#include "../event/Event.h"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentManager& componentManager);

    void update(float deltaTime) override;

    void onEvent(const Event& event);

    void reset() override { return; };
    
private:
    ComponentManager& componentManager;
};