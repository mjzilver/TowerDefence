#pragma once

#include <string>
#include <unordered_map>

#include "../ecs/Component.h"

enum class EventType {
    PROJECTILE_HIT,
    ENTITY_DESTROYED,
    GRASS_TILE_CLICKED,
    TOWER_CLICKED,
    ACTIVATE_CHEATS,
    ACTIVATE_STRESS_TEST,
    OPEN_MENU_CLICKED,
    ENTITY_CLICKED,
    UNSELECT
};

struct Event {
    EventType type;
    std::unordered_map<std::string, Entity> data;

    void addEntity(const std::string& key, Entity value) { data[key] = value; }

    Entity getEntity(const std::string& key) const {
        auto it = data.find(key);
        return it != data.end() ? it->second : INVALID_ENTITY;
    }
};
