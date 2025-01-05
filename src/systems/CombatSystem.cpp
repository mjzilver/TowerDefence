#include "CombatSystem.h"

void CombatSystem::onEvent(const Event& event) {
    auto& eventdispatcher = EventDispatcher::getInstance();

    if (event.type == EventType::ProjectileHit) {
        Entity projectile = *event.getData<Entity>("projectile");
        Entity target = *event.getData<Entity>("target");

        int projectileDamage = componentManager.getComponent<DamageComponent>(projectile)->damage;
        auto* targetHealth = componentManager.getComponent<HealthComponent>(target);

        if (targetHealth) {
            targetHealth->health -= projectileDamage;

            if (targetHealth->health <= 0) {
                // Dispatch an event to remove the entity and play a death animation
                Event deathEvent;
                deathEvent.type = EventType::EntityDestroyed;
                deathEvent.addData<Entity>("entity", &target);
                eventdispatcher.dispatch(deathEvent);

                // Remove components from the entity
                componentManager.removeComponent<VelocityComponent>(target);
                componentManager.removeComponent<PathfindingComponent>(target);
                componentManager.removeComponent<HealthComponent>(target);
                // Set the texture to the dead texture layer
                componentManager.getComponent<TextureComponent>(target)->zIndex = ZLayer::Dead;
            }
        }

        entityManager->destroyEntity(projectile);
        componentManager.removeAllComponents(projectile);
    }
}