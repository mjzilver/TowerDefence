#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>
#include "../tile/Tile.h"
#include "../texture/TextureManager.h"
#include "Map.h"

class MapLoader {
public:
    MapLoader(TextureManager& textureManager);

    Map LoadMap(const std::string& mapName);
private:
    const std::string MAP_PATH = "resources/maps/";
    TextureManager& textureManager;

    std::string readFile(const std::string& filepath);
    
    Map parseMap(const std::string& mapData);
};