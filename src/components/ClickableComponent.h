#pragma once

#include "../ecs/Component.h"
#include "../event/Event.h"

struct ClickableComponent : public Component {
    EventType clickedEvent;
    bool selected = false;
};