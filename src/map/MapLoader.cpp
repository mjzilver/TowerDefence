#include "MapLoader.h"
#include <fstream>
#include <sstream>
#include "../tile/TileFactory.h"

MapLoader::MapLoader(const std::string& mapName, std::vector <Tile> &tiles, TextureManager& textureManager) : textureManager(textureManager) {
    std::string mapData = readFile(MAP_PATH + mapName);
    parseMap(mapData, tiles);
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

void MapLoader::parseMap(const std::string& mapData, std::vector<Tile>& tiles) {
    std::istringstream stream(mapData);
    std::string line;

    int y = 0;
    
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string tileType;
        int x = 0;

        // p = path, g = grass, S = start, E = end
        while (lineStream >> tileType) {
            glm::vec2 position(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE);
            switch (tileType[0]) {
            case 'g':
                tiles.push_back(TileFactory::createGrassTile(position, textureManager));
                break;
            case 'p':
                tiles.push_back(TileFactory::createPathTile(position, textureManager));
                break;
            case 'S':
                tiles.push_back(TileFactory::createStartTile(position, textureManager));
                break;
            case 'E':
                tiles.push_back(TileFactory::createEndTile(position, textureManager));
                break;
            default:
                throw std::runtime_error("Invalid tile type: " + tileType);
            }
            x++;
        }
        y++;
    }

    // TODO loop over to blend corners and edges with transition tiles
}
