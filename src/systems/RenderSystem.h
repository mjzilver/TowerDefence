#pragma once 

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <map>
#include <unordered_map>

#include "../shader/Shader.h"
#include "../ecs/System.h"
#include "../ecs/ComponentManager.h"
#include "../components/PositionComponent.h"
#include "../components/TextureComponent.h"
#include "../components/TextComponent.h"
#include "../components/SizeComponent.h"
#include "../components/RotationComponent.h"
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
        const glm::vec3* color,
        Shader* shader
    );

    void renderText(
        PositionComponent* position, 
        SizeComponent* size,
        TextComponent* text,
        Shader* shader
    );

    void renderText(
        glm::vec4 rect,
        std::string text,
        const glm::vec3& color,
        Shader* shader
    );

    void renderRectangle(
        PositionComponent* position, 
        SizeComponent* size,
        const glm::vec3& color,
        Shader* shader
    );

    void renderRectangle(
        glm::vec4 rect,
        const glm::vec3& color,
        Shader* shader
    );
    
    void render();

    void reset() override { return; };

    void update(float) override {return;};

    void registerShader(const std::string& name, Shader* shader) {
        shaderPrograms[name] = shader;
    }

    Shader* getShader(const std::string& name) {
        return shaderPrograms[name];
    }

private:
    ComponentManager& componentManager;
    FontLoader& fontLoader;

    // hash map to store the shader programs
    std::unordered_map<std::string, Shader*> shaderPrograms;
    std::map<Shader*, GLuint> shaderVAOs; 
};
