#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 offset;
uniform mat4 mvp, model;

out vec3 position, normal;

void main() {
    vec4 instancePosition = vec4(aPos + offset, 1.0);
    position = vec3(model * instancePosition);
    gl_Position = mvp * instancePosition;
    normal = normalize(mat3(transpose(inverse(model))) * aNormal);
}
