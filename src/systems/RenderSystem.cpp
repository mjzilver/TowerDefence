#include "RenderSystem.h"

#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

const int screenWidth = 800;
const int screenHeight = 800;

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

void RenderSystem::renderEntity(
    PositionComponent* position, 
    TextureComponent* texture, 
    SizeComponent* size,
    Shader* shader
) {
    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint shaderProgram = shader->getProgram();

    // Convert from pixel to normalized coordinates
    glm::vec2 normalizedPosition =
        glm::vec2((position->x + size->w * 0.5f) / screenWidth * 2.0f - 1.0f,
                  -(position->y + size->h * 0.5f) / screenHeight * 2.0f + 1.0f);

    // Convert from pixel to normalized size
    glm::vec2 normalizedSize = glm::vec2(size->w / screenWidth * 2.0f,
                                         size->h / screenHeight * 2.0f);

    // offset is x and y divided by the texture size (where the correct texture starts in the atlas)
    glm::vec2 texCoordOffset = glm::vec2(texture->coords.x / texture->texture.size.x,
                                         texture->coords.y / texture->texture.size.y);

    // scaling the texture to fit the tile (how many times the texture fits in the tile)
    glm::vec2 texCoordScale = glm::vec2(texture->coords.z / texture->texture.size.x,
                                        texture->coords.w / texture->texture.size.y);

    // Create the model transformation matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Flip the texture if needed
    if (texture->flipped) {
        model = glm::translate(model, glm::vec3(normalizedPosition.x, normalizedPosition.y, 0.0f)); 
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f)); 
    } else {
        model = glm::translate(model, glm::vec3(normalizedPosition, 0.0f));
    }
    
    model = glm::scale(model, glm::vec3(normalizedSize, 1.0f));

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    GLuint texCoordOffsetLoc = glGetUniformLocation(shaderProgram, "texCoordOffset");
    glUniform2fv(texCoordOffsetLoc, 1, &texCoordOffset[0]);

    GLuint texCoordScaleLoc = glGetUniformLocation(shaderProgram, "texCoordScale");
    glUniform2fv(texCoordScaleLoc, 1, &texCoordScale[0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderSystem::render(Shader* shader) {
    // sort entities by z-index
    std::vector<Entity> sortedEntities(entities.begin(), entities.end());
    std::sort(sortedEntities.begin(), sortedEntities.end(), [this](Entity a, Entity b) {
        auto* positionA = this->componentManager.getComponent<TextureComponent>(a);
        auto* positionB = this->componentManager.getComponent<TextureComponent>(b);

        if (positionA && positionB) {
            return positionA->zIndex > positionB->zIndex;
        }

        return false;
    });

    GLuint shaderProgram = shader->getProgram();
    GLuint VAO;

    if (shaderVAOs.find(shader) == shaderVAOs.end()) {
        // Create a single VAO for this shader if it doesn't already exist
        shaderVAOs[shader] = createUnitSquareVAO();
    } 
    
    VAO = shaderVAOs[shader];
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Render each entity
    for (Entity entity : sortedEntities) {
        auto* position = componentManager.getComponent<PositionComponent>(entity);
        auto* texture = componentManager.getComponent<TextureComponent>(entity);
        auto* size = componentManager.getComponent<SizeComponent>(entity);

        if (position && texture && size) {
            renderEntity(position, texture, size, shader);
        }
    }

    glBindVertexArray(0);
    glUseProgram(0);
}
