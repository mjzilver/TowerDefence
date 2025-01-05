#include "RenderSystem.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../font/FontLoader.h"

#include "../utils/Globals.h"

GLuint createUnitSquareVAO() {
    float vertices[] = {
        // Position          // Texture coordinates
        -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom-left
        0.5f,  -0.5f, 1.0f, 0.0f,  // Bottom-right
        -0.5f, 0.5f,  0.0f, 1.0f,  // Top-left
        0.5f,  0.5f,  1.0f, 1.0f   // Top-right
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

    return VAO;
}

void RenderSystem::renderSquare(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, Shader* shader) {
    GLuint shaderProgram = shader->getProgram();
    glUseProgram(shaderProgram);

    // Set color for the square
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(colorLoc, color.x, color.y, color.z);

    GLuint sizeLoc = glGetUniformLocation(shaderProgram, "bounds");
    glUniform4f(sizeLoc, position.x, position.y, position.x + size.x, position.y + size.y);

    // Create the model transformation matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

    // Check if VAO exists, if not create it
    if (shaderVAOs.find(shader) == shaderVAOs.end()) {
        shaderVAOs[shader] = createUnitSquareVAO();
    }

    // Bind the VAO and draw the square
    glBindVertexArray(shaderVAOs[shader]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderSystem::renderEntity(PositionComponent* position, TextureComponent* texture, SizeComponent* size, RotationComponent* rotation,
                                Shader* shader) {
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

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderSystem::renderText(const std::string& text, const glm::vec2& position, const glm::vec3& color, Shader* shader) {
    glUseProgram(shader->getProgram());
    if (shader->getProgram() == 0) {
        std::cerr << "Shader program not found" << std::endl;
        return;
    }
    // Set text color
    glUniform3f(glGetUniformLocation(shader->getProgram(), "textColor"), color.x, color.y, color.z);

    // Set the projection matrix
    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
    GLuint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    // Ensure VAO and VBO are set up correctly
    static GLuint VAO = 0, VBO = 0;
    if (VAO == 0 || VBO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Start position
    float x = position.x;
    float y = position.y;

    // Get the characters from the font loader
    const std::map<char, Character>& characters = fontLoader.getCharacters();

    // Iterate over the characters in the string
    for (const char& c : text) {
        const Character& ch = characters.at(c);

        if (ch.textureID == 0) {
            std::cerr << "Character not found in font: " << c << std::endl;
        }

        float xpos = x + ch.bearing.x;
        float ypos = y - (ch.bearing.y);

        // Create the vertices using normalized width and height
        float vertices[6][4] = {
            {xpos, ypos + ch.size.y, 0.0f, 1.0f},
            {xpos, ypos, 0.0f, 0.0f},
            {xpos + ch.size.x, ypos, 1.0f, 0.0f},

            {xpos, ypos + ch.size.y, 0.0f, 1.0f},
            {xpos + ch.size.x, ypos, 1.0f, 0.0f},
            {xpos + ch.size.x, ypos + ch.size.y, 1.0f, 1.0f}
        };

        // Render the character
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Upload the vertices to the GPU
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance to the next character position
        x += ch.advance >> 6;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void RenderSystem::renderMenu(MenuComponent* menu, Shader* shader, const glm::vec2& parentPosition) {
    glm::vec2 menuPosition = parentPosition + menu->position;

    float x = menuPosition.x;
    float y = menuPosition.y;
    
    // Render buttons
    for (const Button& button : menu->buttons) {
        glm::vec2 buttonPosition = menuPosition + button.position;

        glm::vec2 buttonTextSize = glm::vec2(button.text.size() * 12, 12);
        glm::vec2 buttonTextPosition = buttonPosition - button.size * 0.5f;
        buttonTextPosition.y += buttonTextSize.y;
        renderText(button.text, buttonTextPosition, glm::vec3(1.0f, 1.0f, 1.0f), shader);

        renderSquare(buttonPosition, button.size, glm::vec3(0.5f, 0.5f, 0.5f), shaderPrograms["menu"]);
    }

    // Render the menu title (first)
    glm::vec2 titleSize = glm::vec2(menu->title.size() * 12, 12);
    glm::vec2 titlePosition = menuPosition - menu->size * 0.5f;
    titlePosition.y += titleSize.y;
    renderText(menu->title, titlePosition, glm::vec3(1.0f, 1.0f, 1.0f), shader);

    // render the menu background
    renderSquare(menuPosition, menu->size, glm::vec3(0.8f, 0.8f, 0.8f), shaderPrograms["menu"]);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderSystem::render() {
    auto entities = getEntities();

    std::vector<Entity> sortedEntities(entities.begin(), entities.end());
    std::sort(sortedEntities.begin(), sortedEntities.end(), [this](Entity a, Entity b) {
        auto* positionA = this->componentManager.getComponent<TextureComponent>(a);
        auto* positionB = this->componentManager.getComponent<TextureComponent>(b);

        if (positionA && positionB) {
            return positionA->zIndex > positionB->zIndex;
        }

        return false;
    });

    for (Entity entity : sortedEntities) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* texture = componentManager.getComponent<TextureComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);
        auto* rotation = componentManager.getComponent<RotationComponent>(entity);
        auto* shaderComponent = componentManager.getComponent<ShaderComponent>(entity);
        auto* menu = componentManager.getComponent<MenuComponent>(entity);

        std::string shaderName = "default";
        if (shaderComponent) {
            shaderName = shaderComponent->name;
        }
        Shader* shader = shaderPrograms[shaderName];
        GLuint shaderProgram = shader->getProgram();
        GLuint VAO;

        if (shaderVAOs.find(shader) == shaderVAOs.end()) {
            shaderVAOs[shader] = createUnitSquareVAO();
        }

        VAO = shaderVAOs[shader];
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        if (position && texture && size) {
            renderEntity(position, texture, size, rotation, shader);
        }

        // Draw menus last
        if (menu && position) {
            glm::vec2 parentPosition = glm::vec2(position->x, position->y);
            renderMenu(menu, shaderPrograms["text"], parentPosition);
        }
    }
    
    glBindVertexArray(0);
    glUseProgram(0);
    
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in RenderSystem::render: " << err << std::endl;
    }
}
