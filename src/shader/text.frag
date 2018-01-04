#version 330 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D text;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = white * sampled;
}
