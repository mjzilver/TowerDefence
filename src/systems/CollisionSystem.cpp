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
        auto* pos = positions->get(entity);
        auto* vel = velocities->get(entity);
        auto* dmg = damages->get(entity);
        auto* size = sizes->get(entity);
        auto* col = collisions->get(entity);

        if (pos && vel && size && dmg && col) {
            float x = pos->x + col->x;
            float y = pos->y + col->y;
            float w = col->w;
            float h = col->h;

            for (Entity otherEntity : collisions->getEntities()) {
                if (entity == otherEntity) {
                    continue;
                }

                auto* otherPos = positions->get(otherEntity);
                auto* otherCol = collisions->get(otherEntity);
                auto* otherHealth = healths->get(otherEntity);

                if (otherPos && otherCol && otherHealth) {
                    float ox = otherPos->x + otherCol->x;
                    float oy = otherPos->y + otherCol->y;
                    float ow = otherCol->w;
                    float oh = otherCol->h;

                    if (checkCollision(x, y, w, h, ox, oy, ow, oh)) {
                        if (col->solid && otherCol->solid) {
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
            if (pos->x + size->w > SCREEN_WIDTH + OOB_MARGIN || pos->x + size->w < -OOB_MARGIN || pos->y + size->h > SCREEN_HEIGHT + OOB_MARGIN ||
                pos->y + size->h < -OOB_MARGIN) {
                componentManager.scheduleDestruction(entity);
            }
        }
    }
}