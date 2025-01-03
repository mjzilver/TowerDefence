#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "enemy/Enemy.h"
#include "enemy/EnemyFactory.h"
#include "map/MapLoader.h"
#include "shader/Shader.h"
#include "systems/RenderSystem.h"
#include "texture/TextureManager.h"
#include "tile/Tile.h"
#include "tile/TileFactory.h"

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

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Shader shader("vertex.glsl", "fragment.glsl");

    MapLoader mapLoader = MapLoader(textureManager);
    Map map = mapLoader.LoadMap("map1.txt");

    RenderSystem renderSystem;

    std::vector<Enemy> enemies;

    enemies.push_back(EnemyFactory::createFireBug(glm::vec2(0, 0), textureManager));

    long long ticks = 0;

    while (!glfwWindowShouldClose(window)) {
        ticks++;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // tick based animation
        if (ticks % enemies[0].frameSpeed == 0) {
            enemies[0].frame = (enemies[0].frame + 1) % enemies[0].frameCount;
            enemies[0].updateTextureCoords();
        }
        
        renderSystem.addRenderBatch(&shader, map.tiles, 0);
        renderSystem.addRenderBatch(&shader, std::vector<Tile>{enemies[0].toTile()}, 1);

        renderSystem.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
