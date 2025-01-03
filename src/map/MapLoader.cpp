#include "MapLoader.h"
#include <fstream>
#include <sstream>
#include "../tile/TileFactory.h"

MapLoader::MapLoader(TextureManager& textureManager) : textureManager(textureManager) {}

Map MapLoader::LoadMap(const std::string& mapName) {
    std::string mapData = readFile(MAP_PATH + mapName);
    return parseMap(mapData);
}

std::string MapLoader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Map MapLoader::parseMap(const std::string& mapData) {
    std::istringstream stream(mapData);
    std::string line;

    Map map;

    int y = 0;
    
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string tileType;
        int x = 0;

        // p = path, g = grass, S = start, E = end
        while (lineStream >> tileType) {
            glm::vec2 position(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE);
            Tile startTile, endTile;
            
            switch (tileType[0]) {
            case 'g':
                map.tiles.push_back(TileFactory::createGrassTile(position, textureManager));
                break;
            case 'p':
                map.tiles.push_back(TileFactory::createPathTile(position, textureManager));
                break;
            case 'S':
                startTile = TileFactory::createStartTile(position, textureManager);
                map.startTile = &startTile;

                map.tiles.push_back(startTile);
                break;
            case 'E':
                endTile = TileFactory::createEndTile(position, textureManager);
                map.endTile = &endTile;

                map.tiles.push_back(endTile);
                break;
            default:
                throw std::runtime_error("Invalid tile type: " + tileType);
            }
            x++;
        }
        y++;
    }

    // TODO loop over to blend corners and edges with transition tiles

    // TODO find path from start to end (a*)

    return map;
}
