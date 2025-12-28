#include "Shader.h"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    const std::string fullVertexPath = shaderPath + vertexPath;
    const std::string fullFragmentPath = shaderPath + fragmentPath;

    std::string vertexCode = readFile(fullVertexPath);
    std::string fragmentCode = readFile(fullFragmentPath);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertexCode.c_str(), vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragmentCode.c_str(), fragmentShader);

    linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::getProgram() const { return programID; }

std::string Shader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::compileShader(const char* shaderSource, GLuint shaderID) {
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);
    checkCompileErrors(shaderID, "SHADER");
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (type != "PROGRAM" && !success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << type << " compilation failed\n" << infoLog << std::endl;
    }
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    for (size_t i = 0; i < static_cast<size_t>(Uniform::COUNT); ++i) {
        uniforms[i] = glGetUniformLocation(programID, UNIFORM_NAMES[i]);
    }
}

GLuint Shader::getUniform(Uniform u) { return uniforms[static_cast<size_t>(u)]; }
