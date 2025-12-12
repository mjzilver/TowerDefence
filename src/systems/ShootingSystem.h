#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

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