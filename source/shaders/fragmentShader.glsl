#version 330 core
in vec3 fragNormal;
in vec2 fragTexCoord;
out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

uniform sampler2D screenTexture;
const float bloomThreshold = 0.1;
const float bloomIntensity = 3.0;

void main() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (diff * lightColor + ambient) * objectColor;
    vec4 screenColor = texture(screenTexture, fragTexCoord);

    result += bloomIntensity * screenColor.rgb;
    fragColor = vec4(result, 1.0);
}
