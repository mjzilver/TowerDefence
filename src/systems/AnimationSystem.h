#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentManager& componentManager) : componentManager(componentManager) {
        EventDispatcher::getInstance().addListener(EventType::ENTITY_DESTROYED, std::bind(&AnimationSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float deltaTime) override;

    void onEvent(const Event& event);

private:
    ComponentManager& componentManager;
};