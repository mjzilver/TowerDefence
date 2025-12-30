#pragma once

#include <typeindex>
#include <vector>

#include "../engine/EngineContext.h"
#include "../utils/Globals.h"

class System {
public:
    std::string name;

    explicit System(EngineContext& ctx, const std::string& n = "baseSystem") : context(ctx), name(n) {}
    virtual ~System() = default;

    virtual void update(const float dt) = 0;
    virtual void reset() = 0;

    std::vector<std::type_index> reads;
    std::vector<std::type_index> writes;

    bool excludeFromBatching = false;

    template <typename T>
    const ComponentArray<T>* readArray() const {
        if constexpr (DEBUG_ENABLED) validateRead<T>();
        return context.componentManager.getArray<T>();
    }

    template <typename T>
    ComponentArray<T>* writeArray() {
        if constexpr (DEBUG_ENABLED) validateWrite<T>();
        return context.componentManager.getArray<T>();
    }

    template <typename T>
    const T* read(Entity e) const {
        if constexpr (DEBUG_ENABLED) validateRead<T>();
        return context.componentManager.getComponent<T>(e);
    }

    template <typename T>
    T* write(Entity e) {
        if constexpr (DEBUG_ENABLED) validateWrite<T>();
        return context.componentManager.getComponent<T>(e);
    }

protected:
    EngineContext& context;

    template <typename T>
    void validateWrite() const {
        if (!hasWrite(typeid(T))) {
            std::cerr << "System '" << name << "' missing WRITE access for component: " << typeid(T).name() << "\n";
        }
    }

    template <typename T>
    void validateRead() const {
        if (!hasRead(typeid(T))) {
            std::cerr << "System '" << name << "' missing READ access for component: " << typeid(T).name() << "\n";
        }
    }

    bool hasRead(const std::type_index& t) const { return std::find(reads.begin(), reads.end(), t) != reads.end(); }

    bool hasWrite(const std::type_index& t) const { return std::find(writes.begin(), writes.end(), t) != writes.end(); }
};
