#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include "../texture/TextureManager.h"
#include "../ecs/EntityFactory.h"

struct Corner {
    glm::vec2 position;
    std::string type;
};

class MapLoader {
public:
    MapLoader(EntityFactory& entityFactory) 
        : entityFactory(entityFactory) {}

    void LoadMap(const std::string& mapName);

    std::vector<glm::vec2> waypoints;
    std::vector<Corner> corners;
private:
    const std::string MAP_PATH = "resources/maps/";
    EntityFactory& entityFactory;

    std::string readFile(const std::string& filepath);
    
    void parseMap(const std::string& mapData);

    void parseMetadata(std::istringstream& stream, std::vector<glm::vec2>& points);
    void parseCorners(std::istringstream& stream, std::vector<Corner>& corners);
};