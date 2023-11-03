# CMakeList.txt : CMake project for main, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("galaxySimulator")

# Use file(GLOB ...) to find all .cpp files in a directory
file(GLOB SOURCE_FILES "*.cpp")

# Add source files to this project's executable.
add_executable (galaxySimulator ${SOURCE_FILES} "include/sphere.h" "include/shader.h" "shader.cpp" "include/camera.h" "include/stb_image.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET galaxySimulator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_link_libraries(galaxySimulator ${CMAKE_SOURCE_DIR}/lib/glad.lib)
target_link_libraries(galaxySimulator ${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Set the source directory for shader files
set(SHADER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/shaders)

# Set the destination directory in the build directory
set(SHADER_DEST_DIR ${CMAKE_BINARY_DIR}/shaders)

# Create a custom command to copy shader files to the build directory
add_custom_command(
    TARGET galaxySimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADER_SOURCE_DIR}
    ${SHADER_DEST_DIR}
)

# Tell CMake where to find the shaders at runtime
configure_file(${SHADER_SOURCE_DIR}/shader_config.h.in ${CMAKE_BINARY_DIR}/shader_config.h)
include_directories(${CMAKE_BINARY_DIR})

# Copy the logo.png file to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources/logo.png DESTINATION ${CMAKE_BINARY_DIR})