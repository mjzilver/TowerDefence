// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "ecs/ComponentManager.h"
#include "ecs/EntityFactory.h"
#include "ecs/EntityManager.h"
#include "ecs/SystemManager.h"
#include "font/FontLoader.h"
#include "map/MapLoader.h"
#include "shader/Shader.h"
#include "systems/AnimationSystem.h"
#include "systems/BuildSystem.h"
#include "systems/ClickSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/CombatSystem.h"
#include "systems/MovementSystem.h"
#include "systems/PathfindingSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ShootingSystem.h"
#include "systems/SpawningSystem.h"
#include "systems/StateSystem.h"
#include "texture/TextureManager.h"
#include "utils/Globals.h"

#if STRESS_TEST
#include "components/ClickableComponent.h"
#include "event/Event.h"
#endif

#define WIREFRAME 0

int main() {
    TextureManager textureManager;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower Defence!", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL();
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Shader defaultShader("default_vertex.glsl", "default_fragment.glsl");
    Shader hoverShader("default_vertex.glsl", "hover_fragment.glsl");
    Shader textShader("text_vertex.glsl", "text_fragment.glsl");
    Shader squareShader("square_vertex.glsl", "square_fragment.glsl");

    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;
    EntityFactory entityFactory(componentManager, entityManager, textureManager);
    FontLoader fontLoader(12);

    MapLoader mapLoader = MapLoader(entityFactory);
    mapLoader.loadMap("map1.txt");

    auto& renderSystem = systemManager.registerSystem<RenderSystem>(&entityManager, componentManager, fontLoader);
    auto& animationSystem = systemManager.registerSystem<AnimationSystem>(&entityManager, componentManager);
    auto& movementSystem = systemManager.registerSystem<MovementSystem>(&entityManager, componentManager);
    auto& pathfindingSystem = systemManager.registerSystem<PathfindingSystem>(&entityManager, componentManager, mapLoader);
    auto& shootingSystem = systemManager.registerSystem<ShootingSystem>(&entityManager, componentManager, entityFactory);
    auto& collisionSystem = systemManager.registerSystem<CollisionSystem>(&entityManager, componentManager);
    auto& combatSystem = systemManager.registerSystem<CombatSystem>(&entityManager, componentManager, entityFactory);
    auto& stateSystem = systemManager.registerSystem<StateSystem>(&entityManager, componentManager);
    auto& clickSystem = systemManager.registerSystem<ClickSystem>(&entityManager, componentManager);
    auto& buildSystem = systemManager.registerSystem<BuildSystem>(&entityManager, componentManager, entityFactory);
    auto& spawningSystem = systemManager.registerSystem<SpawningSystem>(&entityManager, componentManager, entityFactory, pathfindingSystem);

    glfwSetWindowUserPointer(window, &clickSystem);
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        auto clickSystem = static_cast<ClickSystem*>(glfwGetWindowUserPointer(window));
        clickSystem->onClick(button, action, x, y);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        auto clickSystem = static_cast<ClickSystem*>(glfwGetWindowUserPointer(window));
        clickSystem->onHover(x, y);
    });

    pathfindingSystem.generatePath();

#if WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    renderSystem.registerShader("default", &defaultShader);
    renderSystem.registerShader("hover", &hoverShader);
    renderSystem.registerShader("text", &textShader);
    renderSystem.registerShader("square", &squareShader);

#if STRESS_TEST
    for (Entity entity : entityManager.getEntities()) {
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);
        if (clickable && clickable->clickedEvent == EventType::GRASS_TILE_CLICKED) {
            Event event;
            event.type = EventType::BUILD_TOWER;
            event.addData("entity", &entity);
            EventDispatcher::getInstance().dispatch(event);
        }
    }

    for (Entity entity : entityManager.getEntities()) {
        auto* clickable = componentManager.getComponent<ClickableComponent>(entity);
        if (clickable && clickable->clickedEvent == EventType::TOWER_CLICKED) {
            Event event;
            event.type = EventType::UPGRADE_TOWER;
            event.addData("entity", &entity);
            EventDispatcher::getInstance().dispatch(event);
            EventDispatcher::getInstance().dispatch(event);
        }
    }
#endif

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        systemManager.updateSystems(deltaTime);

        renderSystem.render();

        componentManager.flushDestructions(entityManager);

        glfwSwapBuffers(window);
        glfwPollEvents();

        static double lastPrint = 0;
        if (currentTime - lastPrint >= 1.0) {
            lastPrint = currentTime;
        }
    }

    glfwTerminate();
    return 0;
}
