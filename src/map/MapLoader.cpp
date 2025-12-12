#include "MapLoader.h"

#include <fstream>
#include <glm/vec2.hpp>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../utils/Globals.h"

void MapLoader::loadMap(const std::string& mapName) {
    tiles.clear();
    std::string mapData = readFile(mapPath + mapName);
    parseMap(mapData);
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

    // Map grid parsing
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string tileType;
        int x = 0;

        tiles.push_back({});
        auto& row = tiles.back();

        while (lineStream >> tileType) {
            glm::vec2 position(x * TILE_SIZE, y * TILE_SIZE);

            switch (tileType[0]) {
                case 'g':
                    row.push_back({TileType::GRASS, entityFactory.createGrassTile(position)});
                    break;
                case 'p':
                    row.push_back({TileType::PATH, entityFactory.createPathTile(position)});
                    break;
                case 'S':
                    startX = x;
                    startY = y;
                    row.push_back({TileType::START, entityFactory.createStartTile(position)});
                    break;
                case 'E':
                    row.push_back({TileType::END, entityFactory.createEndTile(position)});
                    break;
                default:
                    throw std::runtime_error("Invalid tile type: " + tileType);
            }
            x++;
        }
        y++;
    }

    generatePath();
}

void MapLoader::generatePath() {
    static const int NEIGHBORS[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    int prevX = startX, prevY = startY;
    int currentX = startX, currentY = startY;

    auto current = getTile(currentY, currentX).value();

    path.clear();
    path.push_back({currentY, currentX, current.first, current.second});

    while (current.first != TileType::END) {
        bool foundNext = false;

        for (auto& neighbor : NEIGHBORS) {
            int nextY = currentY + neighbor[0];
            int nextX = currentX + neighbor[1];

            auto neighborTile = getTile(nextY, nextX);
            if (!neighborTile.has_value()) continue;

            if ((neighborTile->first == TileType::PATH || neighborTile->first == TileType::END) && !(nextY == prevY && nextX == prevX)) {
                prevX = currentX;
                prevY = currentY;
                currentX = nextX;
                currentY = nextY;
                current = neighborTile.value();

                path.push_back({currentY, currentX, current.first, current.second});
                foundNext = true;
                break;
            }
        }

        if (!foundNext) {
            throw std::runtime_error("Path is broken or has a loop!");
        }
    }
}

std::optional<std::pair<TileType, Entity>> MapLoader::getTile(int y, int x) const {
    if (y < 0 || y >= (int)tiles.size()) return std::nullopt;
    if (x < 0 || x >= (int)tiles[y].size()) return std::nullopt;
    return tiles[y][x];
}

void MapLoader::debugPrintPath() const {
    std::vector<std::vector<char>> grid;
    for (auto& row : tiles) {
        std::vector<char> line;
        for (auto& tile : row) {
            switch (tile.first) {
                case TileType::GRASS:
                    line.push_back('.');
                    break;
                case TileType::PATH:
                    line.push_back(' ');
                    break;
                case TileType::START:
                    line.push_back('S');
                    break;
                case TileType::END:
                    line.push_back('E');
                    break;
            }
        }
        grid.push_back(line);
    }

    for (auto& row : grid) {
        for (auto c : row) {
            std::cout << c;
        }
        std::cout << '\n';
    }
}