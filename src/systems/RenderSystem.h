#pragma once 

#include <GL/glew.h>
#include <vector>
#include <map>
#include "../shader/Shader.h"
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/SizeComponent.h"

class RenderSystem : public System {
public:
    RenderSystem(ComponentManager& componentManager) 
        : componentManager(componentManager) {}

    void renderEntity(
        PositionComponent* position, 
        TextureComponent* texture, 
        SizeComponent* size,
        Shader* shader
    );

    void render(Shader* shader);

    void update(float deltaTime) override {
        return;
    }

private:
    ComponentManager& componentManager;

    std::map<Shader*, GLuint> shaderVAOs; 
};
