#pragma once

#include <glad/glad.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <map>
#include <unordered_map>

#include "../components/PositionComponent.h"
#include "../components/RotationComponent.h"
#include "../components/SizeComponent.h"
#include "../components/TextComponent.h"
#include "../components/TextureComponent.h"
#include "../ecs/System.h"
#include "../shader/Shader.h"
#include "../utils/Globals.h"

class RenderSystem : public System {
public:
    RenderSystem(EngineContext& ctx) : System(ctx) { projection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f); }

    ~RenderSystem();

    void renderEntity(const PositionComponent* position, const TextureComponent* texture, const SizeComponent* size,
                      const RotationComponent* rotation, const glm::vec3* color, Shader* shader);

    void renderText(const PositionComponent* position, const SizeComponent* size, const TextComponent* text, Shader* shader);

    void renderText(const glm::vec4& rect, const std::string text, const glm::vec3& color, Shader* shader);

    void renderRectangle(const PositionComponent* position, const SizeComponent* size, const glm::vec3& color, Shader* shader);

    void renderRectangle(const glm::vec4& rect, const glm::vec3& color, Shader* shader);

    void render();

    void reset() override {};

    void update(float) override {};

    void registerShader(const std::string& name, Shader* shader) { shaderPrograms[name] = shader; }

    Shader* getShader(const std::string& name) { return shaderPrograms[name]; }

private:
    glm::mat4 projection;

    // hash map to store the shader programs
    std::unordered_map<std::string, Shader*> shaderPrograms;
    std::map<Shader*, GLuint> shaderVAOs;
};
