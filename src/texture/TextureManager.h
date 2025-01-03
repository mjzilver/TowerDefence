#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <GL/glew.h>

#include "Texture.h"

const std::string TEXTURE_PATH = "resources/images/";

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() = default;

    // Load a texture from a PNG file, return the texture ID
    Texture loadTexture(const std::string& filePath);

private:
    // Load a PNG image and return the pixel data
    unsigned char* loadPngImage(const std::string& filePath, unsigned int& width, unsigned int& height);

    std::unordered_map<std::string, Texture> textureCache;
};