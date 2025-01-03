#pragma once

#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint getProgram() const;

private:
    GLuint programID;

    const std::string SHADER_PATH = "resources/shaders/";


    std::string readFile(const std::string& filepath);
    void compileShader(const char* shaderSource, GLuint shaderID);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);
};