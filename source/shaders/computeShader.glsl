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

void main() {
    uint sphereIndex = gl_GlobalInvocationID.x;
    uint sphereCount = spherePositions.length();

    vec3 currentPosition = spherePositions[sphereIndex];
    vec3 acceleration = vec3(0.0);

    for (uint j = 0; j < sphereCount; j++) {
        if (j != sphereIndex) {
            vec3 otherPosition = spherePositions[j];
            vec3 direction = otherPosition - currentPosition;
            float distance = length(direction);

            // Calculate gravitational force using a simple formula
            float forceMagnitude = 1.0 / (distance * distance + epsilon);
            vec3 force = forceMagnitude * direction;

            // Accumulate forces
            acceleration += force;
        }
    }

    // Update velocity and position
    float deltaTime = 0.001;
    vec3 velocity = sphereVelocities[sphereIndex];
    velocity += acceleration * deltaTime;
    currentPosition += velocity * deltaTime;

    // Update the position and velocity of the sphere
    sphereVelocities[sphereIndex] = velocity;
    spherePositions[sphereIndex] = currentPosition;
}
