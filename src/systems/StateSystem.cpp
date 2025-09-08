#include "StateSystem.h"

#include "../components/AnimationComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/ComponentManager.h"

void StateSystem::update(float) {
    for (Entity entity : getEntities()) {
        auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
        auto* animation = componentManager.getComponent<AnimationComponent>(entity);

        if (animation) {
            if (velocity) {
                if (velocity->x != 0 || velocity->y != 0) {
                    animation->state = State::ACTIVE;
                } else {
                    animation->state = State::IDLE;
                }
            }
        }
    }
}