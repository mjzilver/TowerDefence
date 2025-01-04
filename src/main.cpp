#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ctime>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "ecs/ComponentManager.h"
#include "ecs/EntityFactory.h"
#include "ecs/EntityManager.h"
#include "ecs/SystemManager.h"
#include "map/MapLoader.h"
#include "shader/Shader.h"
#include "systems/AnimationSystem.h"
#include "systems/MovementSystem.h"
#include "systems/PathfindingSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ShootingSystem.h"
#include "texture/TextureManager.h"

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

    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;
    EntityFactory entityFactory(componentManager, entityManager, textureManager);

    auto& renderSystem = systemManager.registerSystem<RenderSystem>(&entityManager, componentManager);
    auto& animationSystem = systemManager.registerSystem<AnimationSystem>(&entityManager, componentManager);
    auto& movementSystem = systemManager.registerSystem<MovementSystem>(&entityManager, componentManager);
    auto& pathfindingSystem = systemManager.registerSystem<PathfindingSystem>(&entityManager, componentManager);
    auto& shootingSystem = systemManager.registerSystem<ShootingSystem>(&entityManager, componentManager, entityFactory);
    auto& collisionSystem = systemManager.registerSystem<CollisionSystem>(&entityManager, componentManager);

    // Create the map
    MapLoader mapLoader = MapLoader(entityFactory);
    mapLoader.LoadMap("map1.txt");

    // place a debug tower
    entityFactory.createTower(glm::vec2(400, 100));
    entityFactory.createTower(glm::vec2(400, 500));

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < 5; ++i) {
        float x = static_cast<float>(std::rand() % screenWidth);
        float y = static_cast<float>(std::rand() % screenHeight);
        auto en = entityFactory.createFireBug(glm::vec2(x, y));
    }

    // generate the path (once per map)
    pathfindingSystem.generatePath();

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // cornflower blue
        glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        systemManager.updateSystems(deltaTime);

        // rendersystem is special - needs to be last
        renderSystem.render(&shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // print fps every second
        static double lastPrint = 0;
        if (currentTime - lastPrint >= 1.0) {
            std::cout << "FPS: " << 1.0 / deltaTime << std::endl;
            lastPrint = currentTime;
        }
    }

    glfwTerminate();
    return 0;
}
