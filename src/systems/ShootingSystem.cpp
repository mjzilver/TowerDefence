#include "ShootingSystem.h"

#include "../components/AnimationComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PositionComponent.h"
#include "../components/RotationComponent.h"
#include "../components/SizeComponent.h"
#include "../components/VelocityComponent.h"
#include "../components/WeaponComponent.h"

void ShootingSystem::update(float deltaTime) {
    for (Entity entity : getEntities()) {
        auto* weapon = componentManager.getComponent<WeaponComponent>(entity);
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* rotation = componentManager.getComponent<RotationComponent>(entity);
        auto* animation = componentManager.getComponent<AnimationComponent>(entity);

        if (!weapon || !position) {
            continue;
        }

        weapon->timeSinceLastShot += deltaTime;

        if (weapon->timeSinceLastShot < weapon->rateOfFire) {
            continue;
        }

        for (Entity otherEntity : getEntities()) {
            if (entity == otherEntity) {
                continue;
            }

            auto* otherPosition = componentManager.getComponent<PositionComponent>(otherEntity);
            auto* otherSize = componentManager.getComponent<SizeComponent>(otherEntity);
            auto* otherVelocity = componentManager.getComponent<VelocityComponent>(otherEntity);
            auto* otherHealth = componentManager.getComponent<HealthComponent>(otherEntity);

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

                    entityFactory.createTowerProjectile(startX, startY, targetX, velocity.x, velocity.y, angle, weapon->damage);

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