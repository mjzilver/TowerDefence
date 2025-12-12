#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../ecs/Component.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/System.h"
#include "../map/MapLoader.h"
#include "../components/PathfindingComponent.h"
#include "../components/SizeComponent.h"

class PathfindingSystem : public System {
public:
    PathfindingSystem(ComponentManager& componentManager, MapLoader& mapLoader) : componentManager(componentManager), mapLoader(mapLoader), path(mapLoader.getPath()) {
    }

    void update(float deltaTime) override;
    void generatePath();

    Entity& getStart() {return start;}

    void reset() override { return; };

private:
    void generateRandomOffset(PathfindingComponent* pathfinding, SizeComponent* targetSize);

    std::vector<PathSegment>& path;

    Entity start;
    Entity end;

    ComponentManager& componentManager;
    MapLoader& mapLoader;
};
