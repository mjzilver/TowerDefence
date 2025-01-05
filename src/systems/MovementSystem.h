#pragma once
#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/TextureComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class MovementSystem : public System {
public:
    MovementSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

private:
    ComponentManager& componentManager;
};