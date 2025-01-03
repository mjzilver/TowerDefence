#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <vector>

unsigned char* TextureManager::loadPngImage(const std::string& filePath, unsigned int& width, unsigned int& height) {
    int channels;
    std::string full_path = TEXTURE_PATH + filePath;

    stbi_set_flip_vertically_on_load(true); // Flip PNG images

    stbi_uc* image = stbi_load(full_path.c_str(), (int*)&width, (int*)&height, &channels, STBI_rgb_alpha);
    if (!image) {
        std::cerr << "Failed to load PNG image: " << full_path << std::endl;
        exit(EXIT_FAILURE);
    }

    return image;
}

Texture TextureManager::loadTexture(const std::string& filePath) {
    if (textureCache.find(filePath) != textureCache.end()) {
        std::cout << "Using cached texture: " << filePath << std::endl;
        std::cout << "Texture id: " << textureCache[filePath].id << std::endl;
        return textureCache[filePath];
    }

    unsigned int width, height;
    unsigned char* image = loadPngImage(filePath, width, height);

    std::cout << "Loaded image: " << filePath << " (" << width << "x" << height << ")" << std::endl;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
    Texture result = {textureId, glm::vec2(width, height)};

    textureCache[filePath] = result;
    return result;
}
