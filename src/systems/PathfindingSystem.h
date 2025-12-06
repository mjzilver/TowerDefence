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
    PathfindingSystem(ComponentManager& componentManager, MapLoader& mapLoader) : componentManager(componentManager), mapLoader(mapLoader) {}

    void update(float deltaTime) override;
    void generatePath();

    Entity& getStart() {return start;}

private:
    void generateRandomOffset(PathfindingComponent* pathfinding, SizeComponent* targetSize);

    std::vector<Entity> pathTiles;

    Entity start;
    Entity end;

    ComponentManager& componentManager;
    MapLoader& mapLoader;
};
