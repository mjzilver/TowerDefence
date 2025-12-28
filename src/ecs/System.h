#pragma once

#include <typeindex>
#include <vector>

#include "../engine/EngineContext.h"

class System {
public:
    explicit System(EngineContext& ctx) : context(ctx) {
        reads.push_back(typeid(Entity));
    }
    virtual ~System() = default;

    virtual void update(const float dt) = 0;
    virtual void reset() = 0;

    std::vector<std::type_index> reads;
    std::vector<std::type_index> writes;

    bool excludeFromBatching = false;

protected:
    EngineContext& context;
};
