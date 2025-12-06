#include "CombatSystem.h"

#include "../components/DamageComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PathfindingComponent.h"
#include "../components/TextureComponent.h"
#include "../components/VelocityComponent.h"
#include "../utils/ZLayer.h"

void CombatSystem::onEvent(const Event& event) {
    auto& eventdispatcher = EventDispatcher::getInstance();

    if (event.type == EventType::PROJECTILE_HIT) {
        Entity projectile = *event.getData<Entity>("projectile");
        Entity target = *event.getData<Entity>("target");

        int projectileDamage = componentManager.getComponent<DamageComponent>(projectile)->damage;
        auto* targetHealth = componentManager.getComponent<HealthComponent>(target);

        if (targetHealth) {
            targetHealth->health -= projectileDamage;

            if (targetHealth->health <= 0) {
                Event deathEvent;
                deathEvent.type = EventType::ENTITY_DESTROYED;
                deathEvent.addData<Entity>("entity", &target);
                eventdispatcher.dispatch(deathEvent);

                componentManager.removeComponent<VelocityComponent>(target);
                componentManager.removeComponent<PathfindingComponent>(target);
                componentManager.removeComponent<HealthComponent>(target);
                componentManager.getComponent<TextureComponent>(target)->zIndex = ZLayer::DEAD;
            }
        }

        entityManager->destroyEntity(projectile);
        componentManager.removeAllComponents(projectile);
    }
}