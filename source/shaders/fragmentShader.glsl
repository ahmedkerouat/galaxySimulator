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

    // Define an array of colors for the gradient
    vec3 colors[10];
    colors[9] = vec3(1.0, 1.0, 1.0);
    colors[8] = vec3(0.8, 0.0, 0.8);
    colors[7] = vec3(0.7, 0.0, 0.8);
    colors[6] = vec3(0.6, 0.0, 0.8);
    colors[5] = vec3(0.5, 0.0, 0.7);
    colors[4] = vec3(0.45, 0.0, 0.65);
    colors[3] = vec3(0.4, 0.0, 0.6);
    colors[2] = vec3(0.35, 0.0, 0.55);
    colors[1] = vec3(0.3, 0.0, 0.5);
    colors[0] = vec3(0.25, 0.0, 0.45);

    // Calculate the index for color interpolation
    float colorIndex = concentration * float(9);

    // Calculate the fractional and integer parts
    float fractionalPart = fract(colorIndex);
    int colorIndexLow = int(floor(colorIndex));
    int colorIndexHigh = int(ceil(colorIndex));

    // Interpolate between two adjacent colors
    vec3 interpolatedColor = mix(colors[colorIndexLow], colors[colorIndexHigh], fractionalPart);

    // Add distortion for a dynamic effect
    vec3 distortion = vec3(0.0, 0.1 * sin(20.0 * gl_FragCoord.x), 0.0);

    // Combine the interpolated color, distortion, and concentration factor
    vec3 finalColor = interpolatedColor + distortion * concentration;

    FragColor = vec4(finalColor, 1.0);
}
