#pragma once

#include "../engine/EngineContext.h"

class System {
public:
    explicit System(EngineContext& ctx) : context(ctx) {}
    virtual ~System() = default;

    virtual void update(const float dt) = 0;
    virtual void reset() = 0;

protected:
    EngineContext& context;
};
