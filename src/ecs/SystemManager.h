#pragma once

#include <typeindex>
#include <unordered_map>

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

    void resetSystems() {
        for (auto& [type, system] : systems) {
            system->reset();
        }
    }

    ~SystemManager() {
        for (auto& [type, system] : systems) {
            delete system;
        }
    }

private:
    std::unordered_map<std::type_index, System*> systems;
};