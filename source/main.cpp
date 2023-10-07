#include <iostream>
#include <vector>
#include <sphere.h>
#include <shader.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include "shader_config.h"
#include <camera.h>

float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "galaxySimulator", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD initialization failed" << std::endl;
        return -1;
    }

    // Vertex and fragment shader source code
    std::string vertexShaderSource = loadShaderSource("shaders/vertexShader.glsl");
    std::string fragmentShaderSource = loadShaderSource("shaders/fragmentShader.glsl");
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    const int numberOfSpheres = 1000;

    std::vector<glm::vec3> spherePositions;

    // Generate random positions for the spheres
    for (int i = 0; i < numberOfSpheres; i++) {
        float x = randomFloat(-2.0f, 2.0f);
        float y = randomFloat(-2.0f, 2.0f);
        float z = randomFloat(-2.0f, 2.0f);
        spherePositions.push_back(glm::vec3(x, y, z));
    }

    // Create a sphere
    Sphere star(0.005f, 50, 50);
    const std::vector<float>& sphereVertices = star.getVertices();
    const std::vector<unsigned int>& sphereIndices = star.getIndices();

    // Create and bind VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Set scroll callback function
    glfwSetScrollCallback(window, scroll_callback);

    // Set mouse callback functions
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Set key callback function
    glfwSetKeyCallback(window, key_callback);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Set transformation matrices (view and projection)
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(fov * zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Set uniform variables for lighting
        GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        GLint ambientStrengthLoc = glGetUniformLocation(shaderProgram, "ambientStrength");

        // Set the values for lighting
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(objectColorLoc, 0.8f, 0.0f, 0.8f);
        glUniform1f(ambientStrengthLoc, 0.2f);

        // Draw each sphere at its respective position
        for (const glm::vec3& position : spherePositions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

            GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Draw the sphere
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup and exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
