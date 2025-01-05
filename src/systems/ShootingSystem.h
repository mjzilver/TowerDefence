#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/EntityFactory.h"

#include "../components/TextureComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/DirectionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/WeaponComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/RotationComponent.h"
#include "../components/HealthComponent.h"

#include "../utils/TextureCoords.h"

class ShootingSystem : public System {
public:
    ShootingSystem(ComponentManager& componentManager, EntityFactory& entityFactory)
        : componentManager(componentManager), entityFactory(entityFactory) {}

    void update(float deltaTime) override;
private:
    ComponentManager& componentManager;
    EntityFactory& entityFactory;
};