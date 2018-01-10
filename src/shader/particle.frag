#version 330 core
in vec3 normal, position;

uniform vec3 cameraPos;
out vec4 FragColor;


void main() {

    // Ambient
    float ambientStrength = 0.35f;
    float specularStrength = 0.9f;
    vec4 lightColor = vec4(0.9,0.9,0.9,1.0);
    vec4 objectColor = vec4(0.8,0.8,1.0, 0.5);
    vec3 lightPos = vec3(0, 30, -40);

    // Ambient
    vec4 ambient = ambientStrength * objectColor;
    // Diffuse
    vec3 lightDir = normalize(lightPos - position);

    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * objectColor;
    // Specular
    vec3 viewDir = normalize(cameraPos - position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec4 specular = vec4(0.6,0.6,0.6,1.0) * spec; // assuming bright white light color
    FragColor = ambient + diffuse + specular;
}