#include "StateSystem.h"

#include "../components/AnimationComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/ComponentManager.h"

void StateSystem::update(float) {
    auto* velocities = componentManager.getArray<VelocityComponent>();
    auto* animations = componentManager.getArray<AnimationComponent>();

    for (Entity entity : animations->getEntities()) {
        auto* velocity = velocities->get(entity);
        auto* animation = animations->get(entity);

        if (velocity) {
            if (velocity->x != 0 || velocity->y != 0) {
                animation->state = State::ACTIVE;
            } else {
                animation->state = State::IDLE;
            }
        }
    }
}