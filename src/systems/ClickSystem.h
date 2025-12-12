#pragma once

#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"

#include "../event/Event.h"

class ClickSystem : public System {
public:
    ClickSystem(ComponentManager& componentManager);

    void update(float) override { return; }

    void onClick(int button, int action, double x, double y);

    void onHover(double x, double y);
    
    void onEvent(const Event& event);

    void reset() override { return; };

private:
    const int clickSize = 1;
    ComponentManager& componentManager;

    enum class MenuItem { NONE, UPGRADE_TOWER, BUILD_TOWER };
    MenuItem selectedMenuItem = MenuItem::NONE;
};