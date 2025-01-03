#pragma once

#include <unordered_map>
#include <iostream>

#include "Component.h"

class ComponentManager {
public:
    template <typename T>
    void addComponent(Entity entity, T component) {
        auto& componentMap = getComponentMap<T>();
        componentMap[entity] = std::move(component);
    }

    template <typename T>
    T* getComponent(Entity entity) {
        auto& componentMap = getComponentMap<T>();
        auto it = componentMap.find(entity);
        return it != componentMap.end() ? &it->second : nullptr;
    }

    template <typename T>
    void removeComponent(Entity entity) {
        auto& componentMap = getComponentMap<T>();
        componentMap.erase(entity);
    }

private:
    template <typename T>
    std::unordered_map<Entity, T>& getComponentMap() {
        static std::unordered_map<Entity, T> componentMap;
        return componentMap;
    }
};