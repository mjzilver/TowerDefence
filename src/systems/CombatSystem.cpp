#include "CombatSystem.h"

#include "../components/CollisionComponent.h"
#include "../components/DamageComponent.h"
#include "../components/DeathComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../utils/ZLayer.h"

void CombatSystem::onEvent(const Event& event) {
    auto& eventdispatcher = EventDispatcher::getInstance();

    if (event.type == EventType::PROJECTILE_HIT) {
        Entity projectile = *event.getData<Entity>("projectile");
        Entity target = *event.getData<Entity>("target");

        auto* projectileDamageComp = componentManager.getComponent<DamageComponent>(projectile);
        auto* targetPosition = componentManager.getComponent<PositionComponent>(target);
        auto* targetSize = componentManager.getComponent<SizeComponent>(target);
        auto* targetHealth = componentManager.getComponent<HealthComponent>(target);
        auto* targetDeath = componentManager.getComponent<DeathComponent>(target);
        auto* targetTexture = componentManager.getComponent<TextureComponent>(target);

        if (!projectileDamageComp || !targetPosition || !targetSize || !targetHealth || !targetTexture) {
            return;
        }

        int projectileDamage = projectileDamageComp->damage;
        targetHealth->health -= projectileDamage;

        if (targetHealth->health <= 0) {
            Event deathEvent;
            deathEvent.type = EventType::ENTITY_DESTROYED;
            deathEvent.addData<Entity>("entity", &target);
            eventdispatcher.dispatch(deathEvent);

            if (!targetDeath) {
                DeathComponent deathComp;
                deathComp.hasDied = true;
                deathComp.remainingTime = 0.1f;
                componentManager.addComponent(target, deathComp);
            } else {
                targetDeath->hasDied = true;
            }

            componentManager.removeComponent<VelocityComponent>(target);
            componentManager.removeComponent<PathfindingComponent>(target);
            componentManager.removeComponent<HealthComponent>(target);
            targetTexture->zIndex = ZLayer::DEAD;
        }

        entityFactory.createTowerProjectileImpact({targetPosition->x + targetSize->w / 2, targetPosition->y + targetSize->h / 2});

        DeathComponent deathComponent;
        deathComponent.hasDied = true;
        deathComponent.remainingTime = 0.001f;

        componentManager.removeComponent<CollisionComponent>(projectile);
        componentManager.addComponent(projectile, deathComponent);
    }
}
