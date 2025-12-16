#pragma once

#include <glad/glad.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../ecs/EntityFactory.h"

enum class TileType { START, END, GRASS, PATH };

struct PathSegment {
    int y;
    int x;
    TileType type;
    Entity entity;
};

class MapLoader {
public:
    MapLoader(EntityFactory& entityFactory) : entityFactory(entityFactory) {}

    void loadMap(const std::string& mapName);

    std::optional<std::pair<TileType, Entity>> getTile(int y, int x) const;

    void debugPrintPath() const;
    std::vector<PathSegment>& getPath() { return path; };

    PathSegment getStart() const { return path.at(0); };

private:
    const std::string mapPath = "resources/maps/";
    EntityFactory& entityFactory;

    std::vector<PathSegment> path = {};

    std::string readFile(const std::string& filepath);

    void parseMap(const std::string& mapData);
    void generatePath();

    int startX, startY;

    std::vector<std::vector<std::pair<TileType, Entity>>> tiles;
};