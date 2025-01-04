#pragma once
#include "../components/DirectionComponent.h"
#include "../components/PositionComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/HealthComponent.h"
#include "../components/DamageComponent.h"
#include "../components/RotationComponent.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

class CollisionSystem : public System {
public:
    CollisionSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override {
        for (Entity entity : getEntities()) {
            auto* position = componentManager.getComponent<PositionComponent>(entity);
            auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
            auto* damage = componentManager.getComponent<DamageComponent>(entity);
            auto* size = componentManager.getComponent<SizeComponent>(entity);
            auto* collision = componentManager.getComponent<CollisionComponent>(entity);
            // TODO: handle rotated collision boxes
            auto* rotation = componentManager.getComponent<RotationComponent>(entity);

            if (position && velocity && size && collision && damage) {
                float x = position->x;
                float y = position->y;

                for (Entity otherEntity : getEntities()) {
                    if (entity == otherEntity) {
                        continue;
                    }

                    auto* otherPosition = componentManager.getComponent<PositionComponent>(otherEntity);
                    auto* otherSize = componentManager.getComponent<SizeComponent>(otherEntity);
                    auto* otherCollision = componentManager.getComponent<CollisionComponent>(otherEntity);
                    auto* otherHealth = componentManager.getComponent<HealthComponent>(otherEntity);

                    if (otherPosition && otherSize && otherCollision && otherHealth) {
                        float otherX = otherPosition->x + otherCollision->x;
                        float otherY = otherPosition->y + otherCollision->y;
                        float otherW = otherCollision->w;
                        float otherH = otherCollision->h;

                        if (x < otherX + otherW &&
                            x + size->w > otherX &&
                            y < otherY + otherH &&
                            y + size->h > otherY) {
                            if (collision->solid && otherCollision->solid) {
                                otherHealth->health -= damage->damage;

                                if (otherHealth->health <= 0) {
                                    entityManager->destroyEntity(otherEntity);
                                }

                                // TODO: dispatch event instead of destroying entity
                                entityManager->destroyEntity(entity);

                                break;
                            }
                        }
                    }
                }
            }
            
        }
    }

private:
    ComponentManager& componentManager;
};