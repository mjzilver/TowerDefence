#pragma once

#include <unordered_map>
#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <memory>

#include "Component.h"

class ComponentManager {
public:
    template <typename T>
    void addComponent(Entity entity, T component) {
        components[entity][std::type_index(typeid(T))] = std::make_unique<T>(std::move(component));
    }

    template <typename T>
    T* getComponent(Entity entity) {
        auto it = components.find(entity);
        if (it == components.end()) {
            return nullptr;
        }

        auto it2 = it->second.find(std::type_index(typeid(T)));
        if (it2 == it->second.end()) {
            return nullptr;
        }

        return dynamic_cast<T*>(it2->second.get());
    }

    template <typename T>
    void removeComponent(Entity entity) {
        auto it = components.find(entity);
        if (it == components.end()) {
            return;
        }

        it->second.erase(std::type_index(typeid(T)));
    }

    void removeAllComponents(Entity entity) {
        components.erase(entity);
    }
private:
    // this holds all components
    std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<Component>>> components;
};