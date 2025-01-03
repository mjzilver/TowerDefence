#include "RenderSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

RenderSystem::RenderSystem() {}

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

void RenderSystem::addRenderBatch(Shader* shader, const std::vector<Tile>& tiles, int zIndex) {
    if (shaderVAOs.find(shader) == shaderVAOs.end()) {
        // Create a single VAO for this shader if it doesn't already exist
        shaderVAOs[shader] = createUnitSquareVAO();
    }

    RenderBatch batch;
    batch.shader = shader;
    batch.tiles = tiles;
    batch.zIndex = zIndex;

    renderBatches.push_back(batch);
}

void RenderSystem::render() {
    // Sort render batches by z-index (higher z-index is rendered last so it's on top)
    std::sort(renderBatches.begin(), renderBatches.end(),
              [](RenderBatch& a, RenderBatch& b) { return a.zIndex > b.zIndex; });

    for (const auto& batch : renderBatches) {
        Shader* shader = batch.shader;
        GLuint shaderProgram = shader->getProgram();
        GLuint VAO = shaderVAOs[shader];

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        for (const auto& tile : batch.tiles) {
            // Bind the texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tile.texture.id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // Convert from pixel to normalized coordinates
            glm::vec2 normalizedPosition =
                glm::vec2((tile.position.x + tile.size.x * 0.5f) / screenWidth * 2.0f - 1.0f,
                          -(tile.position.y + tile.size.y * 0.5f) / screenHeight * 2.0f + 1.0f);

            // Convert size to normalized coordinates
            glm::vec2 normalizedSize = glm::vec2(tile.size.x / screenWidth * 2.0f, tile.size.y / screenHeight * 2.0f);

            // offset is x and y divided by the texture size (where the correct texture starts in the atlas)
            glm::vec2 texCoordOffset =
                glm::vec2(tile.textureCoords.x / tile.texture.size.x, tile.textureCoords.y / tile.texture.size.y);

            // coord scale helps with scaling the texture to fit the tile (how many times the texture fits in the tile)
            glm::vec2 texCoordScale =
                glm::vec2(tile.textureCoords.z / tile.texture.size.x, tile.textureCoords.w / tile.texture.size.y);

            // Send transformation to the shader
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(normalizedPosition, 0.0f));
            model = glm::scale(model, glm::vec3(normalizedSize, 1.0f));

            GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

            GLuint texCoordOffsetLoc = glGetUniformLocation(shaderProgram, "texCoordOffset");
            glUniform2fv(texCoordOffsetLoc, 1, &texCoordOffset[0]);

            GLuint texCoordScaleLoc = glGetUniformLocation(shaderProgram, "texCoordScale");
            glUniform2fv(texCoordScaleLoc, 1, &texCoordScale[0]);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glBindVertexArray(0);
        glUseProgram(0);
    }

    renderBatches.clear();
}
