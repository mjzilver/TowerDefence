#pragma once

#include <unordered_map>
#include <typeindex>
#include "System.h"

class SystemManager {
public:
    template <typename T, typename... Args>
    T& registerSystem(EntityManager* em, Args&&... args) {
        T* system = new T(std::forward<Args>(args)...);
        system->setEntityManager(em);
        systems[std::type_index(typeid(T))] = std::move(system);
        return *system;
    }

    template <typename T>
    T* getSystem() {
        auto it = systems.find(std::type_index(typeid(T)));
        return it != systems.end() ? dynamic_cast<T*>(it->second) : nullptr;
    }

    void updateSystems(float deltaTime) {
        for (auto& [type, system] : systems) {
            system->update(deltaTime);
        }
    }

private:
    std::unordered_map<std::type_index, System*> systems;
};