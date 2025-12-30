#include "StateSystem.h"

#include "../components/AnimationComponent.h"
#include "../components/VelocityComponent.h"

StateSystem::StateSystem(EngineContext& ctx) : System(ctx, "StateSystem") {
    context.eventDispatcher.addListener(EventType::OPEN_MENU_CLICKED, std::bind(&StateSystem::onEvent, this, std::placeholders::_1));

    writes.push_back(typeid(AnimationComponent));

    reads.push_back(typeid(VelocityComponent));
}

void StateSystem::update(float) {
    auto& componentManager = context.componentManager;

    const auto* velocities = readArray<VelocityComponent>();
    auto* animations = writeArray<AnimationComponent>();

    for (Entity entity : animations->getEntities()) {
        const auto* velocity = velocities->get(entity);
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
