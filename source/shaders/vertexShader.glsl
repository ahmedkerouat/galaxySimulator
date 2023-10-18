#version 430 core

layout(location = 0) in vec3 inPosition;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculate the final position of the quad
    vec4 worldPosition = model * vec4(inPosition, 1.0);
    gl_Position = projection * view * worldPosition;
    
    // Calculate texture coordinates for mapping the sphere texture
    TexCoords = inPosition.xy * 0.5 + 0.5;
}
