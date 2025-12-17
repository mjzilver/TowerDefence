#pragma once

#include <glad/glad.h>

#include <string>
#include <array>
#include "Uniform.h"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint getProgram() const;
    GLuint getUniform(Uniform u);

    ~Shader() {
        if (programID != 0) {
            glDeleteProgram(programID);
            programID = 0;
        }
    }

private:
    GLuint programID;
    std::array<GLuint, static_cast<size_t>(Uniform::COUNT)> uniforms;

    const std::string shaderPath = "resources/shaders/";

    std::string readFile(const std::string& filepath);
    void compileShader(const char* shaderSource, GLuint shaderID);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
};
