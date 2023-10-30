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
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

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
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

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

    std::string computeShaderSource = loadShaderSource("shaders/computeShader.glsl");
    GLuint computeProgram = createComputeShaderProgram(computeShaderSource);

    // Variables to control
    int numberOfSpheres = 10000;
    float epsilon = 0.01;
    float darkMatterMass = 10.0; // Mass of dark matter
    float darkEnergyAcceleration = 0.0001; // Acceleration due to dark energy
    float deltaTime = 0.0001;
    float supermassiveBlackHoleMass = 1000.0;
    bool isPaused = false;

    std::vector<glm::vec3> spherePositions;
    std::vector<glm::vec3> sphereVelocities;

    for (int i = 0; i < numberOfSpheres; i++) {
        float radius = static_cast<float>(i) / numberOfSpheres * 1.20f;
        float angle = static_cast<float>(i) * 1.0f;

        float x = radius * cos(angle);
        float y = radius * sin(angle);
        float z = randomFloat(-0.2f, 0.2f);

        spherePositions.push_back(glm::vec3(x, y, z));
        sphereVelocities.push_back(glm::vec3(0.0f));
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

    GLuint positionBuffer, velocityBuffer;
    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &velocityBuffer);

    // Initialize buffer data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec3) * spherePositions.size(), spherePositions.data(), GL_DYNAMIC_COPY);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec3) * sphereVelocities.size(), sphereVelocities.data(), GL_DYNAMIC_COPY);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main render loop
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("galaxySimulator");
        ImGui::Text("Welcome to the galaxySimulator!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Use the compute program and bind SSBOs
        glUseProgram(computeProgram);
        glUniform1f(glGetUniformLocation(computeProgram, "epsilon"), epsilon);
        glUniform1f(glGetUniformLocation(computeProgram, "darkMatterMass"), darkMatterMass);
        glUniform1f(glGetUniformLocation(computeProgram, "darkEnergyAcceleration"), darkEnergyAcceleration);
        glUniform1f(glGetUniformLocation(computeProgram, "deltaTime"), deltaTime);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocityBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBuffer);
        // Dispatch the compute shader
        glDispatchCompute(numberOfSpheres, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        // Unbind SSBOs
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

        // Read back data from the SSBO
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBuffer);
        void* positionData = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3) * numberOfSpheres, GL_MAP_READ_BIT);
        // Use the data to update spherePositions and sphereVelocities
        for (int i = 0; i < numberOfSpheres; i++) {
            glm::vec3* position = (glm::vec3*)((char*)positionData + i * sizeof(glm::vec3));
            spherePositions[i] = *position;

        }
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        // Switch to the rendering shader program
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
        glUniform1f(ambientStrengthLoc, 0.8f);


        // Draw each sphere at its respective position
        for (const glm::vec3& position : spherePositions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

            GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Pass the sphere's position to the fragment shader
            GLuint positionLoc = glGetUniformLocation(shaderProgram, "spherePosition");
            glUniform3fv(positionLoc, 1, glm::value_ptr(position));

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}