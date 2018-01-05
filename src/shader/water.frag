#version 330 core
uniform samplerCube skybox;
uniform vec3 cameraPos;

in vec3 normal;
in vec3 position;
uniform mat4 model, view, projection;
out vec4 FragColor;

void main() {
    vec3 I = normalize(position - cameraPos);
    mat3 inv = mat3(inverse(view));
    vec3 R_reflect = inv * reflect(I, normal);
    vec4 color_reflect = texture(skybox, R_reflect);
    float ratio = 1.00 / 1.52;
    vec3 R_refract = inv * refract(I, normal, ratio);
    vec4 color_refract = texture(skybox, R_refract);
    //FragColor = texture(skybox, I) * 0.7;
    //FragColor = color_refract*0.7;
    //FragColor = color_reflect * 0.7;
    FragColor = mix(color_reflect, color_refract, 0.8);

}
