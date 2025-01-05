#include "MapLoader.h"

#include <fstream>
#include <glm/vec2.hpp>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../utils/Globals.h"

void MapLoader::LoadMap(const std::string& mapName) {
    std::string mapData = readFile(MAP_PATH + mapName);
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
        if (line.find("#metadata") != std::string::npos) {
            break;
        }

        std::istringstream lineStream(line);
        std::string tileType;
        int x = 0;

        while (lineStream >> tileType) {
            glm::vec2 position(x * TILE_SIZE, y * TILE_SIZE);

            switch (tileType[0]) {
                case 'g':
                    entityFactory.createGrassTile(position);
                    break;
                case 'p':
                    entityFactory.createPathTile(position);
                    break;
                case 'S':
                    entityFactory.createStartTile(position);
                    break;
                case 'E':
                    entityFactory.createEndTile(position);
                    break;
                default:
                    throw std::runtime_error("Invalid tile type: " + tileType);
            }
            x++;
        }
        y++;
    }

    // Parsing metadata section
    while (std::getline(stream, line)) {
        if (line.find("waypoints:") != std::string::npos) {
            parseMetadata(stream, waypoints);
        } else if (line.find("corners:") != std::string::npos) {
            parseCorners(stream, corners);
        }
    }
}

void MapLoader::parseMetadata(std::istringstream& stream, std::vector<glm::vec2>& points) {
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty() || line.find("#end") != std::string::npos) {
            break;
        }

        int x, y;
        if (sscanf(line.c_str(), "[%d, %d]", &x, &y) == 2) {
            points.push_back(glm::vec2(x, y));
        } else {
            throw std::runtime_error("Invalid waypoint format: " + line);
        }
    }
}

void MapLoader::parseCorners(std::istringstream& stream, std::vector<Corner>& corners) {
    std::string line;
    // Regex pattern to match [x, y] and corner type
    std::regex pattern(R"(\[(\d+),\s*(\d+)\]\s*([a-zA-Z\-]+))");
    std::smatch match;

    while (std::getline(stream, line)) {
        if (line.empty() || line.find("#end") != std::string::npos) {
            break;
        }

        if (std::regex_search(line, match, pattern)) {
            int x = std::stoi(match[1].str());  // Extract x coordinate
            int y = std::stoi(match[2].str());  // Extract y coordinate
            std::string type = match[3].str();  // Extract corner type (e.g., "left-top")

            corners.push_back({glm::vec2(x, y), type});
        } else {
            throw std::runtime_error("Invalid corner format: " + line);
        }
    }
}
