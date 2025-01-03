#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "shader/Shader.h"
#include "texture/TextureManager.h"
#include "tile/TileFactory.h"
#include "systems/RenderSystem.h"

const int screenWidth = 800;
const int screenHeight = 800;

int main() {
    TextureManager textureManager;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Synergy Towers", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("vertex.glsl", "fragment.glsl");

    std::vector<std::vector<Tile>> tiles;
    for (int i = 0; i < 10; ++i) {
        std::vector<Tile> row;
        for (int j = 0; j < 10; ++j) {
            row.push_back(TileFactory::createGrassTile(glm::vec2(i * 80, j * 80), textureManager));
        }
        tiles.push_back(row);
    }

    RenderSystem renderSystem(shader);

    // texture filtering (otherwise causes lines)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        renderSystem.renderTiles(tiles);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
