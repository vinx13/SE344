#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//uniform mat4 model;

vec3 normal, position;
uniform mat4 model, view, projection;

out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
    position = vec3((model * vec4(aPos.xyz,1.0)).xyz);
    normal = mat3(transpose(inverse(model)))*aNormal;
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view*model)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 1.0)));
}
