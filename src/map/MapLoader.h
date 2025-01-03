#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>
#include "../texture/TextureManager.h"
#include "../systems/EntityFactory.h"

class MapLoader {
public:
    MapLoader(TextureManager& textureManager, EntityFactory& entityFactory) 
        : textureManager(textureManager), entityFactory(entityFactory) {}

    void LoadMap(const std::string& mapName);
private:
    const std::string MAP_PATH = "resources/maps/";
    TextureManager& textureManager;
    EntityFactory& entityFactory;

    std::string readFile(const std::string& filepath);
    
    void parseMap(const std::string& mapData);
};