#version 330

in vec3 fragNormal;
in vec3 fragPosition;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
    vec3 lightColor = vec3(1.0);
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);

    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ambient
    vec3 ambient = 0.1 * lightColor;

    fragColor = vec4(ambient + diffuse, 1.0);
}
