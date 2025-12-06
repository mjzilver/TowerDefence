#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D textureSampler;

uniform bool useRecolor;
uniform vec3 recolor; 

void main() {
    vec4 texColor = texture(textureSampler, TexCoord);

    if (texColor.a < 0.1) {
        discard;
    }

    if (useRecolor) {
        texColor.rgb *= recolor;
    }

    FragColor = texColor;
}
