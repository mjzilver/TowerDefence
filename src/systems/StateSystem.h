#pragma once
#include "../components/DamageComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/ZLayer.h"

class StateSystem : public System {
public:
    StateSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
};