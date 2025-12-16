#pragma once

#include "../ecs/System.h"

enum class EngineState { MAIN_MENU, GAMEPLAY };

class StateSystem : public System {
public:
    StateSystem(EngineContext& ctx) : System(ctx) {
        context.eventDispatcher.addListener(EventType::OPEN_MENU_CLICKED, std::bind(&StateSystem::onEvent, this, std::placeholders::_1));
    }

    void update(float deltaTime) override;

    void reset() override {};

    void onEvent(const Event& event);

    EngineState& getState();

    void openMainMenu();

    void startGame();

private:
    EngineState engineState = EngineState::MAIN_MENU;
};