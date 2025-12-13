#pragma once

#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"

class ClickSystem : public System {
public:
    ClickSystem(ComponentManager& componentManager);

    void update(float) override { return; }

    void onClick(int button, int action, double x, double y);

    void onHover(double x, double y);
    
    void reset() override { return; };

private:
    ComponentManager& componentManager;
};