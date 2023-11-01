#version 430

layout(local_size_x = 128) in;

layout(std430, binding = 0) buffer SpherePositions {
    vec3 spherePositions[];
};

layout(std430, binding = 1) buffer SphereVelocities {
    vec3 sphereVelocities[];
};

uniform float epsilon;
uniform float darkMatterMass;
uniform float darkEnergyAcceleration;
uniform float deltaTime;
uniform float supermassiveBlackHoleMass;

const vec3 supermassiveBlackHolePosition = vec3(0.0, 0.0, 0.0);

void main() {
    uint sphereIndex = gl_GlobalInvocationID.x;
    uint sphereCount = spherePositions.length();

    if (sphereIndex >= sphereCount) {
        return;
    }

    vec3 currentPosition = spherePositions[sphereIndex];
    vec3 acceleration = vec3(0.0);

    // Calculate gravitational force due to dark matter and dark energy
    for (uint j = 0; j < sphereCount; j++) {
        if (j != sphereIndex) {
            vec3 direction = spherePositions[j] - currentPosition;
            float distanceSq = dot(direction, direction);
            float forceMagnitudeDarkMatter = darkMatterMass / (distanceSq + epsilon);
            acceleration += forceMagnitudeDarkMatter * direction;
        }
    }
    acceleration += darkEnergyAcceleration;

    // Calculate gravitational force due to the black hole
    vec3 blackHoleDirection = supermassiveBlackHolePosition - currentPosition;
    float blackHoleDistanceSq = dot(blackHoleDirection , blackHoleDirection);   

    float forceMagnitudeBlackHole = supermassiveBlackHoleMass / (blackHoleDistanceSq + epsilon);
    vec3 forceBlackHole = (forceMagnitudeBlackHole * blackHoleDirection);

    acceleration += forceBlackHole;

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
