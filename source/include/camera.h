float zoom = 1.0f;
float zoomSensitivity = 0.1f;
bool rightMousePressed = false;
bool leftMousePressed = false;
bool spaceKeyPressed = false;
glm::vec2 lastMousePos;
float yaw = -9.0f;
float pitch = 0.0f;
float fov = 45.0f;
float cameraSpeed = 0.005f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom -= yoffset * zoomSensitivity;

    // zoom limit
    zoom = glm::clamp(zoom, 0.1f, 2.0f);
}

// handle camera rotation and position change
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;

    if (rightMousePressed) {
        if (firstMouse) {
            lastMousePos.x = static_cast<float>(xpos);
            lastMousePos.y = static_cast<float>(ypos);
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos) - lastMousePos.x;
        float yoffset = lastMousePos.y - static_cast<float>(ypos);

        lastMousePos.x = static_cast<float>(xpos);
        lastMousePos.y = static_cast<float>(ypos);

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Limit pitch to prevent camera flipping
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }
    else if (leftMousePressed) {
        float xoffset = lastMousePos.x - static_cast<float>(xpos);
        float yoffset = lastMousePos.y - static_cast<float>(ypos);

        lastMousePos.x = static_cast<float>(xpos);
        lastMousePos.y = static_cast<float>(ypos);

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // Adjust camera position based on mouse movement
        cameraPosition += glm::vec3(-xoffset, yoffset, 0.0f) * cameraSpeed;
    }
    else {
        firstMouse = true;
    }
}

// Mouse button callback function
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftMousePressed = true;
        }
        else if (action == GLFW_RELEASE) {
            leftMousePressed = false;
        }
    }
}

// Key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        // Reset the view to normal
        yaw = -90.0f;
        pitch = 0.0f;
        zoom = 1.0f;
        cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    }
}