#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <map>
#include <string>

struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class FontLoader {
public:
    FontLoader(GLuint fontSize);

    ~FontLoader() {
        for (auto& [c, charData] : characters) {
            glDeleteTextures(1, &charData.textureID);
        }
    }

    const std::map<char, Character>& getCharacters() const { return characters; }

private:
    FT_Library ft;
    FT_Face face;
    std::map<char, Character> characters;

    const std::string fontPath = "resources/font/DejaVuSansMono.ttf";
};
