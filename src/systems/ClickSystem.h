#pragma once
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"

class ClickSystem : public System {
public:
    ClickSystem(ComponentManager& componentManager) : componentManager(componentManager) {}

    void update(float) override { return; }

    void onClick(int button, int action, double x, double y);

    void onHover(double x, double y);
    
private:
    const int CLICK_SIZE = 1;
    ComponentManager& componentManager;
};