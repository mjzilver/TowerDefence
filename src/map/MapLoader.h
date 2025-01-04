#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>
#include "../texture/TextureManager.h"
#include "../ecs/EntityFactory.h"

class MapLoader {
public:
    MapLoader(EntityFactory& entityFactory) 
        : entityFactory(entityFactory) {}

    void LoadMap(const std::string& mapName);
private:
    const std::string MAP_PATH = "resources/maps/";
    EntityFactory& entityFactory;

    std::string readFile(const std::string& filepath);
    
    void parseMap(const std::string& mapData);
};