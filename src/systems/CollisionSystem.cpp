#include "CollisionSystem.h"

#include "../components/CollisionComponent.h"
#include "../components/DamageComponent.h"
#include "../components/DeathComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/VelocityComponent.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/Globals.h"

void CollisionSystem::update(float) {
    auto& eventdispatcher = EventDispatcher::getInstance();

    for (Entity entity : getEntities()) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
        auto* damage = componentManager.getComponent<DamageComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* collision = componentManager.getComponent<CollisionComponent>(entity);

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
                            event.type = EventType::PROJECTILE_HIT;
                            event.addData("projectile", &entity);
                            event.addData("target", &otherEntity);
                            eventdispatcher.dispatch(event);

                            break;
                        }
                    }
                }
            }

            // Schedule removal when Out Of Bounds
            static const float OOB_MARGIN = 100.0f;
            if (position->x + size->w > SCREEN_WIDTH + OOB_MARGIN || position->x + size->w < -OOB_MARGIN ||
                position->y + size->h > SCREEN_HEIGHT + OOB_MARGIN || position->y + size->h < -OOB_MARGIN) {
                DeathComponent deathComponent;
                deathComponent.hasDied = true;
                deathComponent.remainingTime = 0.1f;

                componentManager.addComponent(entity, deathComponent);
            }
        }
    }
}