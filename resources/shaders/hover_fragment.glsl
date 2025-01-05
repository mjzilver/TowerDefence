#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D textureSampler;

void main() {
    vec4 texColor = texture(textureSampler, TexCoord);

    // Alpha = transparency
    if (texColor.a < 0.1) {
        discard;
    }

    // shift the color to be darker
    texColor.r = texColor.r * 0.8;
    texColor.g = texColor.g * 0.8;
    texColor.b = texColor.b * 0.8;

    FragColor = texColor;
}