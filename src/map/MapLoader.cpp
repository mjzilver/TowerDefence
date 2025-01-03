#include "MapLoader.h"
#include <fstream>
#include <sstream>

void MapLoader::LoadMap(const std::string& mapName) {
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

void MapLoader::parseMap(const std::string& mapData) {
    std::istringstream stream(mapData);
    std::string line;
    int y = 0;
    
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string tileType;
        int x = 0;

        // p = path, g = grass, S = start, E = end
        while (lineStream >> tileType) {
            static const int TILE_SIZE = 80;

            glm::vec2 position(x * TILE_SIZE, y * TILE_SIZE);

            switch (tileType[0]) {
            case 'g':
                entityFactory.createGrassTile(position, textureManager);
                break;
            case 'p':
                entityFactory.createPathTile(position, textureManager);
                break;
            case 'S':
                entityFactory.createStartTile(position, textureManager);
                break;
            case 'E':
                entityFactory.createEndTile(position, textureManager);
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
}
