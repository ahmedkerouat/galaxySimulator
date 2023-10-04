# CMakeList.txt : CMake project for main, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("galaxySimulator")

# Add source to this project's executable.
add_executable (galaxySimulator "main.cpp" "include/sphere.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET galaxySimulator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_link_libraries(galaxySimulator ${CMAKE_SOURCE_DIR}/lib/glad.lib)
target_link_libraries(galaxySimulator ${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)
