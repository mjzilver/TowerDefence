#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include "System.h"

class SystemManager {
public:
    template <typename T, typename... Args>
    T& registerSystem(EntityManager* em, Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* systemPtr = system.get();
        systemPtr->SetEntityManager(em);
        systems[std::type_index(typeid(T))] = std::move(system);
        return *systemPtr;
    }

    template <typename T>
    T* getSystem() {
        auto it = systems.find(std::type_index(typeid(T)));
        return it != systems.end() ? dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    void updateSystems(float deltaTime) {
        for (auto& [type, system] : systems) {
            system->update(deltaTime);
        }
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> systems;
};