#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;

void main() {
    // 0 z and 1 w
    vec4 worldPosition = vec4(aPos, 0.0, 1.0);
    gl_Position = model * worldPosition;
    TexCoord = aTexCoord;
}
