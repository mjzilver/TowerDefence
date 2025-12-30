#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../components/PathfindingComponent.h"
#include "../ecs/Component.h"
#include "../ecs/System.h"
#include "../map/MapLoader.h"

class PathfindingSystem : public System {
public:
    PathfindingSystem(EngineContext& ctx);

    void update(float deltaTime) override;
    void generatePath();
    void reset() override {};

    Entity& getStart() { return start; }

private:
    void generateRandomOffset(PathfindingComponent* pathfinding, const float maxOffset);

    std::vector<PathSegment>& path;

    Entity start;
    Entity end;

    const float maxOffset = 20.0f;
    const int targetCheckSteps = 3;
    const float targetReachThreshold = 3.0f;
};
