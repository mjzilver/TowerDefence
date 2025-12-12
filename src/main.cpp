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
    bool menuMode = true;
    GLContext glContext;

    GLFWwindow* window = glContext.initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower Defence!");
    glContext.setupOpenGL();

    Shader defaultShader("default_vertex.glsl", "default_fragment.glsl");
    Shader hoverShader("default_vertex.glsl", "hover_fragment.glsl");
    Shader textShader("text_vertex.glsl", "text_fragment.glsl");
    Shader squareShader("square_vertex.glsl", "square_fragment.glsl");

    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;

    TextureManager textureManager;
    EntityFactory entityFactory(componentManager, entityManager, textureManager);
    FontLoader fontLoader(12);
    MapLoader mapLoader(entityFactory);

    Menu menu;
    menu.createMainMenu(mapLoader, componentManager, systemManager, menuMode, window);

    auto& renderSystem = systemManager.registerSystem<RenderSystem>(&entityManager, componentManager, fontLoader);
    auto& animationSystem = systemManager.registerSystem<AnimationSystem>(&entityManager, componentManager);
    auto& movementSystem = systemManager.registerSystem<MovementSystem>(&entityManager, componentManager);
    auto& pathfindingSystem = systemManager.registerSystem<PathfindingSystem>(&entityManager, componentManager, mapLoader);
    auto& shootingSystem = systemManager.registerSystem<ShootingSystem>(&entityManager, componentManager, entityFactory);
    auto& collisionSystem = systemManager.registerSystem<CollisionSystem>(&entityManager, componentManager);
    auto& combatSystem = systemManager.registerSystem<CombatSystem>(&entityManager, componentManager, entityFactory);
    auto& stateSystem = systemManager.registerSystem<StateSystem>(&entityManager, componentManager);
    auto& clickSystem = systemManager.registerSystem<ClickSystem>(&entityManager, componentManager);
    auto& menuSystem = systemManager.registerSystem<MenuSystem>(&entityManager, componentManager, entityFactory, menuMode);
    auto& spawningSystem = systemManager.registerSystem<SpawningSystem>(&entityManager, componentManager, entityFactory, mapLoader);

    InputContext inputCtx{&clickSystem, &menu, &menuMode};
    installInputCallbacks(window, &inputCtx);

    renderSystem.registerShader("default", &defaultShader);
    renderSystem.registerShader("hover", &hoverShader);
    renderSystem.registerShader("text", &textShader);
    renderSystem.registerShader("square", &squareShader);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (menuMode)
            menu.render(renderSystem);
        else {
            systemManager.updateSystems(dt);
            renderSystem.render();
        }

        componentManager.flushDestructions(entityManager);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
