#pragma once
#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/HealthComponent.h"
#include "../components/DamageComponent.h"
#include "../components/RotationComponent.h"
#include "../event/EventDispatcher.h"
#include "../event/Event.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class CollisionSystem : public System {
public:
    CollisionSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;
    
private:
    bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
        return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
    }

    ComponentManager& componentManager;
};