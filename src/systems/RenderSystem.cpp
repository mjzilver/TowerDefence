#include "RenderSystem.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "../components/ClickableComponent.h"
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

void RenderSystem::renderSquare(PositionComponent* position, SizeComponent* size, const glm::vec3& color, Shader* shader) {
    GLuint shaderProgram = shader->getProgram();
    glUseProgram(shaderProgram);

    // Set color for the square
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(colorLoc, color.x, color.y, color.z);

    GLuint sizeLoc = glGetUniformLocation(shaderProgram, "bounds");
    glUniform4f(sizeLoc, position->x, position->y, position->x + size->w, position->y + size->h);

    // Create the model transformation matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position->x + size->w * 0.5f, position->y + size->h * 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(size->w, size->h, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    // Check if VAO exists, if not create it
    if (shaderVAOs.find(shader) == shaderVAOs.end()) {
        shaderVAOs[shader] = createUnitSquareVao();
    }

    // Bind the VAO and draw the square
    glBindVertexArray(shaderVAOs[shader]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderSystem::renderEntity(PositionComponent* position, TextureComponent* texture, SizeComponent* size, RotationComponent* rotation,
                                const glm::vec3* color, Shader* shader) {
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint shaderProgram = shader->getProgram();

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

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    GLuint texCoordOffsetLoc = glGetUniformLocation(shaderProgram, "texCoordOffset");
    glUniform2fv(texCoordOffsetLoc, 1, &texCoordOffset[0]);

    GLuint texCoordScaleLoc = glGetUniformLocation(shaderProgram, "texCoordScale");
    glUniform2fv(texCoordScaleLoc, 1, &texCoordScale[0]);

    if (color) {
        glUniform1i(glGetUniformLocation(shaderProgram, "useRecolor"), GL_TRUE);
        glUniform3f(glGetUniformLocation(shaderProgram, "recolor"), color->r, color->g, color->b);
    } else {
        glUniform1i(glGetUniformLocation(shaderProgram, "useRecolor"), GL_FALSE);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderSystem::renderText(PositionComponent* position, SizeComponent* size, TextComponent* text, Shader* shader) {
    GLuint shaderProgram = shader->getProgram();
    glUseProgram(shaderProgram);

    // Set the text color uniform
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), text->color.x, text->color.y, text->color.z);

    // Set projection matrix (orthographic)
    glm::mat4 projection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

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

    std::vector<std::string> lines = splitLines(text->text);

    float lineSpacing = 8.0f;
    float maxCharHeight = characters.at('A').size.y;

    // Compute starting Y so all lines are vertically centered
    float totalHeight = lines.size() * maxCharHeight + (lines.size() - 1) * lineSpacing;
    float startY = position->y + size->h * 0.5f - totalHeight * 0.5f;

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
        float x = position->x + size->w * 0.5f - lineWidth * 0.5f;

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
            glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);

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

void RenderSystem::render() {
    auto entities = getEntities();

    std::vector<Entity> sortedEntities(entities.begin(), entities.end());
    std::sort(sortedEntities.begin(), sortedEntities.end(), [this](Entity a, Entity b) {
        auto* textureA = this->componentManager.getComponent<TextureComponent>(a);
        auto* textureB = this->componentManager.getComponent<TextureComponent>(b);

        if (textureA && textureB) {
            if (textureA->zIndex == textureB->zIndex) {
                auto* positionA = this->componentManager.getComponent<PositionComponent>(a);
                auto* positionB = this->componentManager.getComponent<PositionComponent>(b);

                return positionA->y > positionB->y;
            }

            return textureA->zIndex > textureB->zIndex;
        }

        return false;
    });

    for (Entity entity : sortedEntities) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* texture = componentManager.getComponent<TextureComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* rotation = componentManager.getComponent<RotationComponent>(entity);
        auto* shaderComponent = componentManager.getComponent<ShaderComponent>(entity);
        auto* textComponent = componentManager.getComponent<TextComponent>(entity);
        auto* colorComponent = componentManager.getComponent<ColorComponent>(entity);
        auto* clickableComponent = componentManager.getComponent<ClickableComponent>(entity);

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

        // Texture rendering
        if (position && texture && size) {
            renderEntity(position, texture, size, rotation, colorComponent ? &colorComponent->color : nullptr, shader);
        }

        // Button rendering
        if (position && textComponent && shader && size && colorComponent) {
            renderText(position, size, textComponent, shader);

            glm::vec3 color = colorComponent->color;
            if (clickableComponent && clickableComponent->selected) {
                color.r = colorComponent->color.r * 1.5f;
                color.g = colorComponent->color.g * 1.5f;
                color.b = colorComponent->color.b * 1.5f;
            }

            renderSquare(position, size, color, shaderPrograms["square"]);
        }
    }

    glBindVertexArray(0);
    glUseProgram(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in RenderSystem::render: " << err << std::endl;
    }
}
