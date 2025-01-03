#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>
#include "../tile/Tile.h"
#include "../texture/TextureManager.h"

class MapLoader {
public:
    MapLoader(const std::string& mapName, std::vector<Tile>& tiles, TextureManager& textureManager);

private:
    const std::string MAP_PATH = "resources/maps/";

    TextureManager& textureManager;

    std::string readFile(const std::string& filepath);
    
    void parseMap(const std::string& mapData, std::vector<Tile>& tiles);
};