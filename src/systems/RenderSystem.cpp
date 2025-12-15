#include "RenderSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "../components/ClickableComponent.h"
#include "../components/CollisionComponent.h"
#include "../components/ColorComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/TextComponent.h"
#include "../font/FontLoader.h"
#include "../utils/Globals.h"
#include "../utils/String.h"

GLuint createUnitSquareVao() {
    float vertices[] = {
        // Position          // Texture coordinates
        -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom-left
        0.5f,  -0.5f, 1.0f, 0.0f,  // Bottom-right
        -0.5f, 0.5f,  0.0f, 1.0f,  // Top-left
        0.5f,  0.5f,  1.0f, 1.0f   // Top-right
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);

    return vao;
}

void RenderSystem::renderRectangle(glm::vec4 rect, const glm::vec3& color, Shader* shader) {
    GLuint shaderProgram = shader->getProgram();
    glUseProgram(shaderProgram);

    GLuint colorLoc = shader->getUniform("color");
    glUniform3f(colorLoc, color.x, color.y, color.z);

    GLuint sizeLoc = shader->getUniform("bounds");
    glUniform4f(sizeLoc, rect.x, rect.y, rect.x + rect.z, rect.y + rect.w);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(rect.x + rect.z * 0.5f, rect.y + rect.w * 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(rect.z, rect.w, 1.0f));

    glUniformMatrix4fv(shader->getUniform("model"), 1, GL_FALSE, &model[0][0]);

    GLuint projectionLoc = shader->getUniform("projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    if (shaderVAOs.find(shader) == shaderVAOs.end()) {
        shaderVAOs[shader] = createUnitSquareVao();
    }

    glBindVertexArray(shaderVAOs[shader]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderSystem::renderRectangle(PositionComponent* position, SizeComponent* size, const glm::vec3& color, Shader* shader) {
    renderRectangle(glm::vec4(position->x, position->y, size->w, size->h), color, shader);
}

void RenderSystem::renderEntity(PositionComponent* position, TextureComponent* texture, SizeComponent* size, RotationComponent* rotation,
                                const glm::vec3* color, Shader* shader) {
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Convert from pixel to normalized coordinates
    glm::vec2 normalizedPosition =
        glm::vec2((position->x + size->w * 0.5f) / SCREEN_WIDTH * 2.0f - 1.0f, -(position->y + size->h * 0.5f) / SCREEN_HEIGHT * 2.0f + 1.0f);

    // Convert from pixel to normalized size
    glm::vec2 normalizedSize = glm::vec2(size->w / SCREEN_WIDTH * 2.0f, size->h / SCREEN_HEIGHT * 2.0f);

    // offset is x and y divided by the texture size (where the correct texture starts in the atlas)
    glm::vec2 texCoordOffset = glm::vec2(texture->coords.x / texture->texture.size.x, texture->coords.y / texture->texture.size.y);

    // scaling the texture to fit the tile (how many times the texture fits in the tile)
    glm::vec2 texCoordScale = glm::vec2(texture->coords.z / texture->texture.size.x, texture->coords.w / texture->texture.size.y);

    // Create the model transformation matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Apply translation
    model = glm::translate(model, glm::vec3(normalizedPosition, 0.0f));

    // Apply rotation (if provided)
    if (rotation) {
        float angleRadians = glm::radians(rotation->angle);  // Convert degrees to radians
        model = glm::rotate(model, angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    // Apply scaling
    model = glm::scale(model, glm::vec3(normalizedSize, 1.0f));

    // Apply flipping (if needed)
    if (texture->flipped) {
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));  // Flip horizontally
    }

    GLuint modelLoc = shader->getUniform("model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    GLuint texCoordOffsetLoc = shader->getUniform("texCoordOffset");
    glUniform2fv(texCoordOffsetLoc, 1, &texCoordOffset[0]);

    GLuint texCoordScaleLoc = shader->getUniform("texCoordScale");
    glUniform2fv(texCoordScaleLoc, 1, &texCoordScale[0]);

    if (color) {
        glUniform1i(shader->getUniform("useRecolor"), GL_TRUE);
        glUniform3f(shader->getUniform("recolor"), color->r, color->g, color->b);
    } else {
        glUniform1i(shader->getUniform("useRecolor"), GL_FALSE);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderSystem::renderText(glm::vec4 rect, std::string text, const glm::vec3& color, Shader* shader) {
    GLuint shaderProgram = shader->getProgram();
    glUseProgram(shaderProgram);

    // Set the text color uniform
    glUniform3f(shader->getUniform("textColor"), color.x, color.y, color.z);

    // Set projection matrix (orthographic)
    glUniformMatrix4fv(shader->getUniform("projection"), 1, GL_FALSE, &projection[0][0]);

    // Setup VAO/VBO
    static GLuint vao = 0, vbo = 0;
    if (vao == 0) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const auto& characters = fontLoader.getCharacters();

    std::vector<std::string> lines = splitLines(text);

    float lineSpacing = 8.0f;
    float maxCharHeight = characters.at('A').size.y;

    // Compute starting Y so all lines are vertically centered
    float totalHeight = lines.size() * maxCharHeight + (lines.size() - 1) * lineSpacing;
    float startY = rect.y + rect.w * 0.5f - totalHeight * 0.5f;

    float y = startY;

    for (const std::string& line : lines) {
        // Compute line width for horizontal centering
        float lineWidth = 0.0f;
        for (char c : line) {
            auto it = characters.find(c);
            if (it != characters.end()) {
                lineWidth += it->second.advance >> 6;
            }
        }

        // Center horizontally inside the bounding box
        float x = rect.x + rect.z * 0.5f - lineWidth * 0.5f;

        // Draw characters in this line
        for (char c : line) {
            auto it = characters.find(c);
            if (it == characters.end()) continue;
            const Character& ch = it->second;

            float xpos = x + ch.bearing.x;
            float ypos = y - ch.bearing.y;

            float w = ch.size.x;
            float h = ch.size.y;

            float vertices[6][4] = {{xpos, ypos + h, 0.0f, 1.0f}, {xpos, ypos, 0.0f, 0.0f},     {xpos + w, ypos, 1.0f, 0.0f},
                                    {xpos, ypos + h, 0.0f, 1.0f}, {xpos + w, ypos, 1.0f, 0.0f}, {xpos + w, ypos + h, 1.0f, 1.0f}};

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glUniform1i(shader->getUniform("text"), 0);

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += ch.advance >> 6;
        }

        // Move to next line
        y += maxCharHeight + lineSpacing;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderSystem::renderText(PositionComponent* position, SizeComponent* size, TextComponent* text, Shader* shader) {
    renderText(glm::vec4(position->x, position->y, size->w, size->h), text->text, text->color, shader);
}

void RenderSystem::render() {
    auto* positions = componentManager.getArray<PositionComponent>();
    auto* textures = componentManager.getArray<TextureComponent>();
    auto* sizes = componentManager.getArray<SizeComponent>();
    auto* rotations = componentManager.getArray<RotationComponent>();
    auto* shaders = componentManager.getArray<ShaderComponent>();
    auto* textComp = componentManager.getArray<TextComponent>();
    auto* colors = componentManager.getArray<ColorComponent>();
    auto* clickable = componentManager.getArray<ClickableComponent>();
    auto* collisions = componentManager.getArray<CollisionComponent>();

    const auto& layers = entityManager->getLayered();

    for (auto layerIt = layers.rbegin(); layerIt != layers.rend(); ++layerIt) {
        auto entities = layerIt->second;

        for (Entity entity : entities) {
            auto* position = positions->get(entity);
            auto* texture = textures->get(entity);
            auto* size = sizes->get(entity);
            auto* rotation = rotations->get(entity);
            auto* shaderComponent = shaders->get(entity);
            auto* textComponent = textComp->get(entity);
            auto* colorComponent = colors->get(entity);
            auto* clickableComponent = clickable->get(entity);
            auto* collision = collisions->get(entity);

            std::string shaderName = "default";
            if (shaderComponent) {
                shaderName = shaderComponent->name;
            }
            Shader* shader = shaderPrograms[shaderName];
            GLuint shaderProgram = shader->getProgram();
            GLuint vao;

            if (shaderVAOs.find(shader) == shaderVAOs.end()) {
                shaderVAOs[shader] = createUnitSquareVao();
            }

            vao = shaderVAOs[shader];
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);

            // Button rendering
            if (position && shader && size && colorComponent && !texture) {
                if (textComponent) {
                    renderText(position, size, textComponent, shader);
                }

                glm::vec3 color = colorComponent->color;
                if (clickableComponent && clickableComponent->selected) {
                    color.r = colorComponent->color.r * 1.5f;
                    color.g = colorComponent->color.g * 1.5f;
                    color.b = colorComponent->color.b * 1.5f;
                }

                renderRectangle(position, size, color, shaderPrograms["rect"]);
            } else if (position && texture && size && shader) {
                // Texture rendering
                renderEntity(position, texture, size, rotation, colorComponent ? &colorComponent->color : nullptr, shader);
            }

            if constexpr (DEBUG_ENABLED) {
                if (!position || !collision) continue;

                glm::vec4 rect{position->x + collision->x, position->y + collision->y, collision->w, collision->h};
                glm::vec3 color{1.0f, 0.0f, 0.0f};

                renderRectangle(rect, color, shaderPrograms["rect"]);
            }
        }
    }

    glBindVertexArray(0);
    glUseProgram(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in RenderSystem::render: " << err << std::endl;
    }
}

RenderSystem::RenderSystem(ComponentManager& componentManager, FontLoader& fontLoader) : componentManager(componentManager), fontLoader(fontLoader) {
    projection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
}
