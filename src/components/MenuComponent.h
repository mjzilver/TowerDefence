
#pragma once

#include "../ecs/Component.h"
#include "../event/Event.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Button {
    std::string text;
    glm::vec2 position;
    glm::vec2 size;
    
    EventType event;
    Entity entity;
};

struct MenuComponent : public Component {
    std::string title;
    glm::vec2 position;
    glm::vec2 size;
    std::vector<Button> buttons;

    bool visible = true;
};