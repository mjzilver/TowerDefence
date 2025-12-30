#include "ShootingSystem.h"

#include "../components/AnimationComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RotationComponent.h"
#include "../components/SizeComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/WeaponComponent.h"

ShootingSystem::ShootingSystem(EngineContext& ctx) : System(ctx) {
    writes.push_back(typeid(WeaponComponent));
    writes.push_back(typeid(RotationComponent));
    writes.push_back(typeid(AnimationComponent));

    reads.push_back(typeid(PositionComponent));
    reads.push_back(typeid(SizeComponent));
    reads.push_back(typeid(VelocityComponent));
    reads.push_back(typeid(HealthComponent));
}

void ShootingSystem::update(float deltaTime) {
    auto& componentManager = context.componentManager;
    auto* weapons = componentManager.getArray<WeaponComponent>();
    const auto* positions = componentManager.getArray<PositionComponent>();
    const auto* sizes = componentManager.getArray<SizeComponent>();
    const auto* velocities = componentManager.getArray<VelocityComponent>();
    const auto* rotations = componentManager.getArray<RotationComponent>();
    const auto* animations = componentManager.getArray<AnimationComponent>();
    auto* healths = componentManager.getArray<HealthComponent>();

    for (Entity entity : weapons->getEntities()) {
        auto* weapon = weapons->get(entity);
        const auto* position = positions->get(entity);
        const auto* size = sizes->get(entity);
        auto* rotation = rotations->get(entity);
        auto* animation = animations->get(entity);

        if (!weapon || !position) {
            continue;
        }

        weapon->timeSinceLastShot += deltaTime;

        if (weapon->timeSinceLastShot < weapon->rateOfFire) {
            continue;
        }

        for (Entity otherEntity : healths->getEntities()) {
            if (entity == otherEntity) {
                continue;
            }

            const auto* otherPosition = positions->get(otherEntity);
            const auto* otherSize = sizes->get(otherEntity);
            const auto* otherVelocity = velocities->get(otherEntity);
            const auto* otherHealth = healths->get(otherEntity);

            if (otherPosition && otherSize && otherVelocity && otherHealth) {
                float startX = position->x + (size->w / 2);
                float startY = position->y + (size->h / 2);

                float otherX = otherPosition->x + (otherSize->w / 2);
                float otherY = otherPosition->y + (otherSize->h / 2);

                float dx = otherX - startX;
                float dy = otherY - startY;

                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < weapon->range) {
                    float timeToTarget = distance / weapon->projectileSpeed;
                    float targetX = otherX + otherVelocity->x * timeToTarget;
                    float targetY = otherY + otherVelocity->y * timeToTarget;

                    float dxTarget = targetX - startX;
                    float dyTarget = targetY - startY;
                    float distanceToTarget = std::sqrt(dxTarget * dxTarget + dyTarget * dyTarget);

                    glm::vec2 velocity;
                    velocity.x = (dxTarget / distanceToTarget) * weapon->projectileSpeed;
                    velocity.y = (dyTarget / distanceToTarget) * weapon->projectileSpeed;
                    float angle = glm::degrees(std::atan2(velocity.x, velocity.y)) + 180;

                    context.entityFactory.createTowerProjectile(startX, startY, targetX, velocity.x, velocity.y, angle, weapon->damage);

                    // rotate the weapon to face the target
                    if (rotation) {
                        rotation->angle = angle;
                    }

                    if (animation) {
                        animation->state = State::SHOOTING;
                        animation->frame = 0;
                    }

                    weapon->timeSinceLastShot = 0.0f;

                    break;
                }
            }
        }
    }
}
