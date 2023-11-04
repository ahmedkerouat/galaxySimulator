# galaxySimulator

<div align="center">
  <a href="https://github.com/ahmedkerouat/galaxySimulator">
    <img src="source\resources\galaxySimulator.jpg" alt="Logo">
  </a>
</div>

## Table of content

- [About the project](#aboutproject)
- [Getting Started](#getstarted)
- [Structure](#structure)
- [Usage](#usage)
- [Controls](#controls)
- [Screenshots](#screenshots)
- [Contributing](#contributing)
- [License](#license)

<a name="aboutproject"></a>

## About the project

This project is a n-body-type simulation of a galaxy. It utilizes a compute shader for real-time calculations of gravitational interactions between randomly generated spheres acting as stars. The simulation, built using OpenGL and GLFW, provides an environment to observe the effects of gravity, dark matter, and dark energy on celestial bodies as they move and interact in a virtual space. For this project, I was inspired by Angel Luriot. 🌌

<a name="getstarted"></a>

## Getting Started

### Dependencies

- git (https://git-scm.com/downloads)
- cmake (https://cmake.org/download)
- make (http://ftp.gnu.org/gnu/make)
- g++ (https://gcc.gnu.org) _you can use another compiler if you want_
- glfw (https://www.glfw.org/)
- glad (https://glad.dav1d.de/)
- glm (https://glm.g-truc.net/)
- imgui (https://github.com/ocornut/imgui)
- stb (https://github.com/nothings/stb/) _used stb_image for icon_

### Installation

```shell
> git clone https://github.com/ahmedkerouat/galaxySimulator.git
> cd galaxySimulator
> mkdir build
> cd build
> cmake ..
> make
> ./galaxySimulator
```

<a name="structure"></a>

## Structure

The project consists of multiple header files, each providing specific functionalities. Here's an overview of the key components:

- **[sphere.h](source/include/sphere.h)**: This header file contains the Sphere class, which is used for rendering stars. It allows customization of the number of vertices and triangles that make up a sphere.

- **[shader.h](source/include/shader.h)**: The shader.h file provides functions for loading and compiling OpenGL shaders and creating shader programs. For instance, it includes a utility function, loadShaderSource, to load shader source code from external files.

- **[shader_config.h](source/shaders/shader_config.h.in)**: This header includes the configuration data for fragment & vertex shaders.

- **[camera.h](source/include/camera.h)**: The Camera class defined in this header is responsible for controlling the view and perspective within the simulation. It manages user input for camera movements.

<a name="usage"></a>

## Usage

You can control various aspects of the simulation through the user interface.

- **Number of Spheres**: Adjust the number of randomly generated celestial spheres.

- **Radius**: Customizes the radius of the galaxy.

- **Epsilon**: A simulation parameter that limits the interaction between the spheres.

- **Delta Time**: Specifies the time step for the simulation.

- **Dark Matter Mass**: Sets the mass of dark matter within the simulation.

- **Dark Energy Effect**: Determines the acceleration due to dark energy.

- **Pause/Resume**: Pauses and resumes the simulation.

<a name="controls"></a>

## Controls

Within the simulation, you can control diverse camera parameters :

- **Right Mouse Button** : Controls the camera's rotation.

- **Left Mouse Button** : Controls the camera's position.

- **Scroll**: Zoom in and out of the simulation.

- **Space button** : Resets camera position to default.

_On the menu Ctrl + Lmb to change precisely a simulation parameter using the slider_

<a name="screenshots"></a>

## Screenshots

<div align="center">
    <img src="source\resources\Screenshot_1.png" alt="1">
    <img src="source\resources\Screenshot_2.png" alt="2">
</div>

<a name="contributing"></a>

## Contributing

If you have a suggestion to improve the project, please feel free to create a pull request. Alternatively, you can open an issue to discuss your ideas.

<a name="license"></a>

## License

Project is licensed under the [MIT LICENSE](https://choosealicense.com/licenses/mit/)
