#pragma once

#include "../ecs/Component.h"
#include "../event/Event.h"
#include <functional>

enum class ClickableType {
    EVENT,
    FUNCTION
};

struct ClickableComponent : public Component {
    bool hovered = false;
    bool selected = false;

    EventType clickedEvent;
    ClickableType type = ClickableType::EVENT;

    std::function<void()> onClick;
};
