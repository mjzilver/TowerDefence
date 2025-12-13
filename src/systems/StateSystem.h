#pragma once

#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"

enum class EngineState {
    MAIN_MENU,
    GAMEPLAY
};


class StateSystem : public System {
public:
    StateSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float deltaTime) override;

    void reset() override { return; };

    EngineState& getState();

    void openMainMenu();

    void startGame();

private:
    ComponentManager& componentManager;
    EngineState engineState = EngineState::MAIN_MENU;
};