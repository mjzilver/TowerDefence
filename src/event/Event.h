#pragma once

#include <string>
#include <unordered_map>

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
    std::unordered_map<std::string, void*> data;

    template <typename T>
    void addData(const std::string& key, T* value) {
        data[key] = static_cast<void*>(value);
    }

    template <typename T>
    T* getData(const std::string& key) const {
        auto it = data.find(key);
        return it != data.end() ? static_cast<T*>(it->second) : nullptr;
    }
};
