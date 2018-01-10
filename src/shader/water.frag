#version 330 core
uniform samplerCube skybox;
uniform vec3 cameraPos;

in vec3 normal;
in vec3 position;
uniform mat4 model, view, projection;
out vec4 FragColor;


void main() {
    vec3 I = normalize(position - cameraPos);

    vec3 R_reflect = reflect(I, normal);

    float cos_theta = abs(dot(I, normal));
    float cos_theta2 = cos_theta*cos_theta;// normal is normalized
    vec4 color_reflect = texture(skybox, R_reflect);
    float ratio = 1.0 / 1.33;
    float r0 = 0.0204;
    float r = clamp(r0 +(1-r0)*cos_theta*cos_theta2*cos_theta2*cos_theta, 0.0, 1.0);

    vec3 R_refract = refract(I, normal, ratio);
    vec4 color_refract = texture(skybox, R_refract);


    vec3 viewDir = normalize(cameraPos - position);
    float spec = 0.0;
    vec3 lightPos = vec3(0, 30, -40);
    vec3 lightDir = normalize(lightPos - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    FragColor = texture(skybox, I);
    FragColor = mix(color_reflect, color_refract,r);
    FragColor *= (1+spec);

}
