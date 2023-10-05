#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragTexCoord = texCoord;
}
