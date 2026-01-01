#pragma once

#include <typeindex>
#include <unordered_map>

#include "../thread/ThreadPool.h"
#include "System.h"

class SystemManager {
public:
    explicit SystemManager(EngineContext& ctx) : context(ctx) {}

    template <typename T>
    T& registerSystem() {
        T* system = new T(context);
        systems[std::type_index(typeid(T))] = std::move(system);
        return *system;
    }

    template <typename T>
    T* getSystem() {
        auto it = systems.find(std::type_index(typeid(T)));
        return it != systems.end() ? dynamic_cast<T*>(it->second) : nullptr;
    }

    void updateSystems(float deltaTime) {
        for (auto& batch : batches) {
            for (auto* sys : batch) {
                threadpool.submit([&, sys]() { sys->update(deltaTime); });
            }
            threadpool.join();
        }

        context.eventDispatcher.swapQueues();
        context.eventDispatcher.deliver();
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

    void generateBatches();

private:
    void debugPrintBatches();

    EngineContext& context;
    std::unordered_map<std::type_index, System*> systems;

    std::vector<std::vector<System*>> batches;

    ThreadPool threadpool;
};