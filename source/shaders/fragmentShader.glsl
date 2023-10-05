#version 330 core
in vec3 fragNormal;
in vec2 fragTexCoord;
out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

void main() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (diff * lightColor + ambient) * objectColor;
    fragColor = vec4(result, 1.0);
}
