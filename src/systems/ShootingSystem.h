#pragma once
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"
#include "../ecs/System.h"

class ShootingSystem : public System {
public:
    ShootingSystem(ComponentManager& componentManager, EntityFactory& entityFactory)
        : componentManager(componentManager), entityFactory(entityFactory) {}

    void update(float deltaTime) override;

    void reset() override { return; };

private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
};