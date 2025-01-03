#pragma once 

#include <GL/glew.h>
#include <vector>
#include <map>
#include "../tile/Tile.h"
#include "../shader/Shader.h"
#include "../enemy/Enemy.h"

struct RenderBatch {
    Shader* shader;
    std::vector<GLuint> VAOs;
    std::vector<Tile> tiles;
    int zIndex;
};

class RenderSystem {
public:
    RenderSystem();

    void addRenderBatch(Shader* shader, const std::vector<Tile>& tiles, int zIndex = 1);
    void render();

private:
    std::vector<RenderBatch> renderBatches;
    std::map<Shader*, GLuint> shaderVAOs; 
};
