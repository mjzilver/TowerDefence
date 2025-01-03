#include "RenderSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO
const int screenWidth = 800;
const int screenHeight = 800;

RenderSystem::RenderSystem(Shader& shader) : shader(shader) { }

GLuint RenderSystem::createSquareVAO(const Tile& tile) {
    float vertices[] = {
        // Position          // Texture coordinates
        -0.5f, -0.5f, tile.textureCoords.x / tile.texture.size.x, tile.textureCoords.y / tile.texture.size.y,  // Bottom-left
         0.5f, -0.5f, (tile.textureCoords.x + tile.textureCoords.z) / tile.texture.size.x, tile.textureCoords.y / tile.texture.size.y,  // Bottom-right
        -0.5f,  0.5f, tile.textureCoords.x / tile.texture.size.x, (tile.textureCoords.y + tile.textureCoords.w) / tile.texture.size.y,  // Top-left
         0.5f,  0.5f, (tile.textureCoords.x + tile.textureCoords.z) / tile.texture.size.x, (tile.textureCoords.y + tile.textureCoords.w) / tile.texture.size.y  // Top-right
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return VAO;
}

void RenderSystem::drawSquare(GLuint shaderProgram, GLuint VAO, const Tile& tile) {
    glUseProgram(shaderProgram);

    // Convert from pixel to normalized coordinates for position
    glm::vec2 normalizedPosition = glm::vec2((tile.position.x + tile.size.x * 0.5f) / screenWidth * 2.0f - 1.0f,
                                             -(tile.position.y + tile.size.y * 0.5f) / screenHeight * 2.0f + 1.0f);

    // Convert size to normalized coordinates
    glm::vec2 normalizedSize = glm::vec2(tile.size.x / screenWidth * 2.0f, tile.size.y / screenHeight * 2.0f);

    // Create a model matrix to translate and scale the square
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(normalizedPosition, 0.0f));
    model = glm::scale(model, glm::vec3(normalizedSize, 1.0f));

    GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);

    // Bind the VAO before drawing
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderSystem::renderTiles(const std::vector<Tile>& tiles) {
    for (const auto& tile : tiles) {
        GLuint squareVAO = createSquareVAO(tile);
        drawSquare(shader.getProgram(), squareVAO, tile);
        glDeleteVertexArrays(1, &squareVAO);
    }
}
