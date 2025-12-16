#include "StateSystem.h"

#include "../components/AnimationComponent.h"
#include "../components/VelocityComponent.h"
#include "../ecs/ComponentManager.h"

void StateSystem::update(float) {
    auto& componentManager = context.componentManager;

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

EngineState& StateSystem::getState() { return engineState; };

void StateSystem::startGame() { engineState = EngineState::GAMEPLAY; }

void StateSystem::openMainMenu() { engineState = EngineState::MAIN_MENU; }

void StateSystem::onEvent(const Event& event) {
    if (event.type == EventType::OPEN_MENU_CLICKED) {
        openMainMenu();
    }
}
