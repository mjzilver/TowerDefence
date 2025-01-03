#pragma once

#include <vector>

#include "../tile/Tile.h"

struct Map {
    Tile* startTile;
    Tile* endTile;

    // All tiles in the map
    std::vector<Tile> tiles;

    // path from start to end in order
    std::vector<Tile*> path;
};