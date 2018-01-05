#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 model;

out vec3 normal, position;
uniform mat4 mvp;
void main() {
    position = vec3((model * vec4(aPos.xyz,1.0)).xyz);
    normal = mat3(transpose(inverse(model)))*aNormal;
    gl_Position = mvp * vec4(aPos.xyz, 1.0);
}
