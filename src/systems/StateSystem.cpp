#include "StateSystem.h"

#include "../components/AnimationComponent.h"

void StateSystem::update(float deltaTime) {
    for (Entity entity : getEntities()) {
        auto* velocity = componentManager.getComponent<VelocityComponent>(entity);
        auto* animation = componentManager.getComponent<AnimationComponent>(entity);

        if (animation) {
            if (velocity) {
                if (velocity->x != 0 || velocity->y != 0) {
                    animation->state = State::Active;
                } else {
                    animation->state = State::Idle;
                }
            }
        }
    }
}