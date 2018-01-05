#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal, position;
uniform mat4 mvp;
void main() {
    position = aPos; // suppose model is identity
    normal = aNormal;
    gl_Position = mvp * vec4(aPos.xyz, 1.0);
}
