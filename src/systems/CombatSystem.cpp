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

CombatSystem::CombatSystem(EngineContext& ctx) : System(ctx, "CombatSystem") {
    context.eventDispatcher.addListener(EventType::PROJECTILE_HIT, std::bind(&CombatSystem::onEvent, this, std::placeholders::_1));

    writes.push_back(typeid(PositionComponent));
    writes.push_back(typeid(HealthComponent));
    writes.push_back(typeid(DeathComponent));
    writes.push_back(typeid(VelocityComponent));
    writes.push_back(typeid(PathfindingComponent));
    writes.push_back(typeid(HealthComponent));
    writes.push_back(typeid(CollisionComponent));

    reads.push_back(typeid(DamageComponent));
    reads.push_back(typeid(SizeComponent));
    reads.push_back(typeid(TextureComponent));
}

void CombatSystem::onEvent(const Event& event) {
    auto& eventdispatcher = context.eventDispatcher;
    auto& componentManager = context.componentManager;

    if (event.type == EventType::PROJECTILE_HIT) {
        Entity projectile = event.getEntity("projectile");
        Entity target = event.getEntity("target");

        const auto* projectileDamageComp = read<DamageComponent>(projectile);
        auto* targetPosition = write<PositionComponent>(target);
        const auto* targetSize = read<SizeComponent>(target);
        auto* targetHealth = write<HealthComponent>(target);
        auto* targetDeath = write<DeathComponent>(target);
        const auto* targetTexture = read<TextureComponent>(target);

        if (!projectileDamageComp || !targetPosition || !targetSize || !targetHealth || !targetTexture) {
            return;
        }

        int projectileDamage = projectileDamageComp->damage;
        targetHealth->health -= projectileDamage;

        if (targetHealth->health <= 0) {
            Event deathEvent;
            deathEvent.type = EventType::ENTITY_DESTROYED;
            deathEvent.addEntity("entity", target);
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
            componentManager.removeComponent<CollisionComponent>(target);

            targetPosition->zIndex = ZLayer::DEAD;
            componentManager.reorder(target, ZLayer::DEAD);
        }

        context.entityFactory.createTowerProjectileImpact({targetPosition->x + targetSize->w / 2, targetPosition->y + targetSize->h / 2});

        DeathComponent deathComponent;
        deathComponent.hasDied = true;
        deathComponent.remainingTime = 0.001f;

        componentManager.removeComponent<CollisionComponent>(projectile);
        componentManager.addComponent(projectile, deathComponent);
    }
}
