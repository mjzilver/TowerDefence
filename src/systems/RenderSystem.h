#pragma once 

#include <GL/glew.h>
#include <vector>
#include <map>
#include <unordered_map>

#include "../shader/Shader.h"
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/SizeComponent.h"
#include "../components/RotationComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/MenuComponent.h"
#include "../font/FontLoader.h"

class RenderSystem : public System {
public:
    RenderSystem(ComponentManager& componentManager, FontLoader& fontLoader) 
        : componentManager(componentManager), fontLoader(fontLoader) {}

    void renderEntity(
        PositionComponent* position, 
        TextureComponent* texture, 
        SizeComponent* size,
        RotationComponent* rotation,
        Shader* shader
    );

    void renderMenu(MenuComponent* menu, Shader* shader, const glm::vec2& parentPosition);
    void renderText(const std::string& text, const glm::vec2& position, const glm::vec3& color, Shader* shader);
    void renderSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, Shader* shader);
    
    void render();

    void update(float deltaTime) override {
        return;
    }

    void registerShader(const std::string& name, Shader* shader) {
        shaderPrograms[name] = shader;
    }

private:
    ComponentManager& componentManager;
    FontLoader& fontLoader;

    // hash map to store the shader programs
    std::unordered_map<std::string, Shader*> shaderPrograms;
    std::map<Shader*, GLuint> shaderVAOs; 
};
