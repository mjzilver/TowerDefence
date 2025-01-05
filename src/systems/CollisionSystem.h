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

    void update(float deltaTime) override {
        auto& eventdispatcher = EventDispatcher::getInstance();

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
                        
                        if (checkCollision(x, y, size->w, size->h, otherX, otherY, otherW, otherH)) {
                            if (collision->solid && otherCollision->solid) {
                                Event event;
                                event.type = EventType::ProjectileHit;
                                event.addData("projectile", &entity);
                                event.addData("target", &otherEntity);
                                eventdispatcher.dispatch(event);

                                break;
                            }
                        }
                    }
                }
            }
        }
    }
private:
    bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
        return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
    }

    ComponentManager& componentManager;
};