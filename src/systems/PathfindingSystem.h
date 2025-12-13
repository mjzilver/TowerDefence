#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../ecs/Component.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"
#include "../map/MapLoader.h"
#include "../components/PathfindingComponent.h"

class PathfindingSystem : public System {
public:
    PathfindingSystem(ComponentManager& componentManager, MapLoader& mapLoader);

    void update(float deltaTime) override;
    void generatePath();
    void reset() override { return; };

    Entity& getStart() {return start;}

private:
    void generateRandomOffset(PathfindingComponent* pathfinding, float maxOffset);

    std::vector<PathSegment>& path;

    Entity start;
    Entity end;

    const float maxOffset = 20.0f;

    ComponentManager& componentManager;
    MapLoader& mapLoader;
};
