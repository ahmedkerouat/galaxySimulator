#version 430

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// Define the buffer blocks for sphere positions and velocities
layout(std430, binding = 0) buffer SpherePositions {
    vec3 spherePositions[];
};

layout(std430, binding = 1) buffer SphereVelocities {
    vec3 sphereVelocities[];
};

const float epsilon = 0.001;
const float darkMatterMass = 10.0; // Mass of dark matter
const float darkEnergyAcceleration = 0.0001; // Acceleration due to dark energy
const float deltaTime = 0.001;

const vec3 supermassiveBlackHolePosition = vec3(0.0, 0.0, 0.0);
const float supermassiveBlackHoleMass = 1000.0; // Mass of the supermassive black hole

void main() {
    uint sphereIndex = gl_GlobalInvocationID.x;
    uint sphereCount = spherePositions.length();

    vec3 currentPosition = spherePositions[sphereIndex];
    vec3 acceleration = vec3(0.0);

    // Calculate the force due to the supermassive black hole
    vec3 blackHoleDirection = supermassiveBlackHolePosition - currentPosition;
    float blackHoleDistance = length(blackHoleDirection);
    float forceMagnitudeBlackHole = supermassiveBlackHoleMass / (blackHoleDistance * blackHoleDistance + epsilon);
    vec3 forceBlackHole = forceMagnitudeBlackHole * blackHoleDirection;

    for (uint j = 0; j < sphereCount; j++) {
        if (j != sphereIndex) {
            vec3 otherPosition = spherePositions[j];
            vec3 direction = otherPosition - currentPosition;
            float distance = length(direction);

            // Calculate gravitational force due to dark matter
            float forceMagnitudeDarkMatter = darkMatterMass / (distance * distance + epsilon);
            vec3 forceDarkMatter = forceMagnitudeDarkMatter * direction;

            // Accumulate forces
            acceleration += forceDarkMatter;
        }
    }

    // Add the effect of dark energy
    acceleration += darkEnergyAcceleration;

    // Update velocity and position
    vec3 velocity = sphereVelocities[sphereIndex];
    velocity += acceleration * deltaTime;

    // For orbiting motion, set the velocity perpendicular to the black hole's position
    vec3 perpendicularVelocity = cross(blackHoleDirection, vec3(0.0, 1.0, 0.0));
    velocity += perpendicularVelocity * deltaTime;

    currentPosition += velocity * deltaTime;

    // Update the position and velocity of the sphere
    sphereVelocities[sphereIndex] = velocity;
    spherePositions[sphereIndex] = currentPosition;
}
