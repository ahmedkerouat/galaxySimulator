#version 330 core

uniform vec3 spherePosition;
uniform vec3 cameraPosition;
uniform float concentrationFactor;

out vec4 FragColor;

void main()
{
    // Calculate the distance from the camera to the sphere
    float distance = length(cameraPosition - spherePosition);

    // Define a threshold for the concentration effect
    float threshold = 1.0;

    // Calculate the concentration factor based on distance
    float concentration = 1.0 - min(distance / threshold, 1.0);

    // Define the brightest and darkest colors (RGB values)
    vec3 brightestColor = vec3(1.0, 0.2, 1.0); // bright
    vec3 darkestColor = vec3(0.6, 0.0, 0.6);   // dark

    // Calculate the transition factor
    float transitionFactor = concentration * 0.7;

    // Calculate the final color using a linear interpolation
    vec3 finalColor = mix(darkestColor, brightestColor, transitionFactor);

    FragColor = vec4(finalColor, 1.0);
}
