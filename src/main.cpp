#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "map/MapLoader.h"
#include "shader/Shader.h"
#include "systems/RenderSystem.h"
#include "texture/TextureManager.h"

#include "ecs/EntityManager.h"
#include "ecs/ComponentManager.h"
#include "ecs/SystemManager.h"
#include "ecs/EntityFactory.h"

#include "systems/RenderSystem.h"
#include "systems/AnimationSystem.h"

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
    EntityFactory entityFactory(componentManager, entityManager);

    auto& renderSystem = systemManager.registerSystem<RenderSystem>(componentManager);
    auto& animationSystem = systemManager.registerSystem<AnimationSystem>(componentManager);

    // Create the map
    MapLoader mapLoader = MapLoader(textureManager, entityFactory);
    mapLoader.LoadMap("map1.txt");

    auto en = entityFactory.createFireBug(glm::vec2(100, 100), textureManager);

    animationSystem.addEntity(en);

    // put all the entities in the render system
    for (auto& entity : entityManager.getEntities()) {
        renderSystem.addEntity(entity);
    }

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
