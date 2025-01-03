#pragma once 

#include <GL/glew.h>
#include <vector>
#include "../tile/Tile.h"
#include "../shader/Shader.h"

class RenderSystem {
public:
    RenderSystem(Shader& shader);
    void renderTiles(const std::vector<Tile>& tiles);
    GLuint createSquareVAO(const Tile& tile);
    void drawSquare(GLuint shaderProgram, GLuint VAO, const Tile& tile);

private:
    Shader& shader;
    GLuint VBO;
};
