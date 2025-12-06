#include "CollisionSystem.h"

#include "../components/CollisionComponent.h"
#include "../components/DamageComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/VelocityComponent.h"
#include "../event/Event.h"
#include "../event/EventDispatcher.h"
#include "../utils/Globals.h"

void CollisionSystem::update(float) {
    auto* collisions = componentManager.getArray<CollisionComponent>();
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* sizes = componentManager.getArray<SizeComponent>();
    auto* velocities = componentManager.getArray<VelocityComponent>();
    auto* damages = componentManager.getArray<DamageComponent>();
    auto* healths = componentManager.getArray<HealthComponent>();

    for (Entity entity : collisions->getEntities()) {
        auto* position = positions->get(entity);
        auto* velocity = velocities->get(entity);
        auto* damage = damages->get(entity);
        auto* size = sizes->get(entity);
        auto* collision = collisions->get(entity);

        if (position && velocity && size && damage) {
            float x = position->x;
            float y = position->y;

            for (Entity otherEntity : collisions->getEntities()) {
                if (entity == otherEntity) {
                    continue;
                }

                auto* otherPosition = positions->get(otherEntity);
                auto* otherSize = sizes->get(otherEntity);
                auto* otherCollision = collisions->get(otherEntity);
                auto* otherHealth = healths->get(otherEntity);

                if (otherPosition && otherSize && otherHealth) {
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
                            EventDispatcher::getInstance().dispatch(event);

                            break;
                        }
                    }
                }
            }

            // Schedule removal when Out Of Bounds
            static const float OOB_MARGIN = 100.0f;
            if (position->x + size->w > SCREEN_WIDTH + OOB_MARGIN || position->x + size->w < -OOB_MARGIN ||
                position->y + size->h > SCREEN_HEIGHT + OOB_MARGIN || position->y + size->h < -OOB_MARGIN) {
                componentManager.scheduleDestruction(entity);
            }
        }
    }
}