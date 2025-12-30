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
    quadTree.clear();

    auto& componentManager = context.componentManager;

    auto* collisions = componentManager.getArray<CollisionComponent>();
    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* sizes = componentManager.getArray<SizeComponent>();
    const auto* velocities = componentManager.getArray<VelocityComponent>();
    const auto* damages = componentManager.getArray<DamageComponent>();
    const auto* healths = componentManager.getArray<HealthComponent>();

    for (Entity entity : collisions->getEntities()) {
        auto* pos = positions->get(entity);
        auto* col = collisions->get(entity);

        if (!pos || !col) {
            continue;
        };
        glm::vec4 entityBounds{pos->x + col->x, pos->y + col->y, col->w, col->h};

        quadTree.insert(entity, entityBounds);
    }

    for (Entity entity : collisions->getEntities()) {
        const auto* pos = positions->get(entity);
        const auto* vel = velocities->get(entity);
        const auto* dmg = damages->get(entity);
        const auto* size = sizes->get(entity);
        const auto* col = collisions->get(entity);

        if (pos && vel && size && dmg && col) {
            const float x = pos->x + col->x;
            const float y = pos->y + col->y;
            const float w = col->w;
            const float h = col->h;

            std::vector<Entity> nearby = quadTree.query({x, y, w, h});
            for (Entity otherEntity : nearby) {
                if (entity == otherEntity) {
                    continue;
                }

                const auto* otherPos = positions->get(otherEntity);
                const auto* otherCol = collisions->get(otherEntity);
                const auto* otherHealth = healths->get(otherEntity);

                if (otherPos && otherCol && otherHealth) {
                    const float ox = otherPos->x + otherCol->x;
                    const float oy = otherPos->y + otherCol->y;
                    const float ow = otherCol->w;
                    const float oh = otherCol->h;

                    if (checkCollision(x, y, w, h, ox, oy, ow, oh)) {
                        if (col->solid && otherCol->solid) {
                            Event event;
                            event.type = EventType::PROJECTILE_HIT;
                            event.addEntity("projectile", entity);
                            event.addEntity("target", otherEntity);
                            context.eventDispatcher.dispatch(event);

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