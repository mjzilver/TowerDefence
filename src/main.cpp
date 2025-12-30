#include <iostream>

#include "ecs/EntityFactory.h"
#include "ecs/SystemManager.h"
#include "engine/EngineContext.h"
#include "engine/GLContext.h"
#include "engine/InputRouter.h"
#include "event/EventDispatcher.h"
#include "map/MapLoader.h"
#include "menu/Menu.h"
#include "shader/Shader.h"
#include "systems/AnimationSystem.h"
#include "systems/ClickSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/CombatSystem.h"
#include "systems/DebugSystem.h"
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

    {
        Shader defaultShader("default_vertex.glsl", "default_fragment.glsl");
        Shader hoverShader("default_vertex.glsl", "hover_fragment.glsl");
        Shader textShader("text_vertex.glsl", "text_fragment.glsl");
        Shader rectShader("rect_vertex.glsl", "rect_fragment.glsl");

        EntityManager entityManager;
        ComponentManager componentManager(entityManager);

        TextureManager textureManager;
        EntityFactory entityFactory(componentManager, entityManager, textureManager);
        FontLoader fontLoader(14);
        MapLoader mapLoader(entityFactory);
        EventDispatcher eventDispatcher;

        Menu menu(eventDispatcher);
        EngineContext context{entityManager, componentManager, fontLoader, mapLoader, eventDispatcher, entityFactory};

        SystemManager systemManager(context);

        auto& renderSystem = systemManager.registerSystem<RenderSystem>();
        auto& animationSystem = systemManager.registerSystem<AnimationSystem>();
        auto& movementSystem = systemManager.registerSystem<MovementSystem>();
        auto& pathfindingSystem = systemManager.registerSystem<PathfindingSystem>();
        auto& shootingSystem = systemManager.registerSystem<ShootingSystem>();
        auto& collisionSystem = systemManager.registerSystem<CollisionSystem>();
        auto& combatSystem = systemManager.registerSystem<CombatSystem>();
        auto& stateSystem = systemManager.registerSystem<StateSystem>();
        auto& clickSystem = systemManager.registerSystem<ClickSystem>();
        auto& menuSystem = systemManager.registerSystem<MenuSystem>();
        auto& spawningSystem = systemManager.registerSystem<SpawningSystem>();
        if constexpr (DEBUG_ENABLED) {
            systemManager.registerSystem<DebugSystem>();
        }
        systemManager.generateBatches();

        menu.createMainMenu(mapLoader, systemManager, stateSystem, window);

        InputContext inputCtx{&clickSystem, &menu, &stateSystem};
        installInputCallbacks(window, &inputCtx);

        renderSystem.registerShader("default", &defaultShader);
        renderSystem.registerShader("hover", &hoverShader);
        renderSystem.registerShader("text", &textShader);
        renderSystem.registerShader("rect", &rectShader);

        double lastTime = glfwGetTime();
        const double fixedDt = 1.0f / 60.0f;
        double accumulator = 0;

        while (!glfwWindowShouldClose(window)) {
            double currentTime = glfwGetTime();
            accumulator += currentTime - lastTime;
            static double lastPrint = 0;

            glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            while (accumulator >= fixedDt) {
                if (stateSystem.getState() == EngineState::GAMEPLAY) {
                    systemManager.updateSystems(fixedDt);
                    componentManager.flush(entityManager);
                }
                accumulator -= fixedDt;
            }

            if (stateSystem.getState() == EngineState::MAIN_MENU) {
                menu.render(renderSystem);
            } else if (stateSystem.getState() == EngineState::GAMEPLAY) {
                renderSystem.render();
            }

            glfwSwapBuffers(window);
            glfwPollEvents();

            if constexpr (DEBUG_ENABLED) {
                if (currentTime - lastPrint >= 1.0) {
                    double frameTime = currentTime - lastTime;
                    std::cout << "FPS: " << 1.0 / frameTime << "\n";
                    lastPrint = currentTime;
                }

                lastTime = currentTime;
            }
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}