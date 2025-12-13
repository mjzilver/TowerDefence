#include <iostream>
#include <ostream>
#include "ecs/EntityFactory.h"
#include "ecs/SystemManager.h"
#include "engine/GLContext.h"
#include "engine/InputRouter.h"
#include "map/MapLoader.h"
#include "menu/Menu.h"
#include "shader/Shader.h"
#include "systems/AnimationSystem.h"
#include "systems/ClickSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/CombatSystem.h"
#include "systems/MenuSystem.h"
#include "systems/MovementSystem.h"
#include "systems/PathfindingSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ShootingSystem.h"
#include "systems/SpawningSystem.h"
#include "systems/StateSystem.h"
#include "texture/TextureManager.h"
#include "utils/Globals.h"

int main() {
    GLContext glContext;

    GLFWwindow* window = glContext.initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower Defence!");
    glContext.setupOpenGL();

    Shader defaultShader("default_vertex.glsl", "default_fragment.glsl");
    Shader hoverShader("default_vertex.glsl", "hover_fragment.glsl");
    Shader textShader("text_vertex.glsl", "text_fragment.glsl");
    Shader rectShader("rect_vertex.glsl", "rect_fragment.glsl");

    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;

    TextureManager textureManager;
    EntityFactory entityFactory(componentManager, entityManager, textureManager);
    FontLoader fontLoader(14);
    MapLoader mapLoader(entityFactory);

    Menu menu;

    auto& renderSystem = systemManager.registerSystem<RenderSystem>(&entityManager, componentManager, fontLoader);
    auto& animationSystem = systemManager.registerSystem<AnimationSystem>(&entityManager, componentManager);
    auto& movementSystem = systemManager.registerSystem<MovementSystem>(&entityManager, componentManager);
    auto& pathfindingSystem = systemManager.registerSystem<PathfindingSystem>(&entityManager, componentManager, mapLoader);
    auto& shootingSystem = systemManager.registerSystem<ShootingSystem>(&entityManager, componentManager, entityFactory);
    auto& collisionSystem = systemManager.registerSystem<CollisionSystem>(&entityManager, componentManager);
    auto& combatSystem = systemManager.registerSystem<CombatSystem>(&entityManager, componentManager, entityFactory);
    auto& stateSystem = systemManager.registerSystem<StateSystem>(&entityManager, componentManager);
    auto& clickSystem = systemManager.registerSystem<ClickSystem>(&entityManager, componentManager);
    auto& menuSystem = systemManager.registerSystem<MenuSystem>(&entityManager, componentManager, entityFactory, stateSystem);
    auto& spawningSystem = systemManager.registerSystem<SpawningSystem>(&entityManager, componentManager, entityFactory, mapLoader);

    menu.createMainMenu(mapLoader, componentManager, systemManager, stateSystem, window);

    InputContext inputCtx{&clickSystem, &menu, &stateSystem};
    installInputCallbacks(window, &inputCtx);

    renderSystem.registerShader("default", &defaultShader);
    renderSystem.registerShader("hover", &hoverShader);
    renderSystem.registerShader("text", &textShader);
    renderSystem.registerShader("rect", &rectShader);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (stateSystem.getState() == EngineState::MAIN_MENU)
            menu.render(renderSystem);
        else {
            systemManager.updateSystems(deltaTime);
            renderSystem.render();
        }

        componentManager.flushDestructions(entityManager);

        glfwSwapBuffers(window);
        glfwPollEvents();

        static double lastPrint = 0;
        if (currentTime - lastPrint >= 1.0) {
            std::cout << "FPS: " << 1.0 / deltaTime << "\n";
            lastPrint = currentTime;
        }
    }

    glfwTerminate();
}