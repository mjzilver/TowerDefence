#pragma once

#include "../components/AnimationComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/HealthComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"
#include "../utils/TextureCoords.h"

#include "../event/Event.h"
#include "../event/EventDispatcher.h"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentManager& componentManager) : componentManager(componentManager) {
        EventDispatcher::getInstance().addListener(EventType::EntityDestroyed, std::bind(&AnimationSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float deltaTime) override;

    void onEvent(const Event& event);

private:
    ComponentManager& componentManager;
};