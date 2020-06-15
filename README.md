# Graphics Programming

This is a personal project, programmed in C++ and using various resources found in text or on the Internet. The goal is to learn about various aspects of graphics programming and working with APIs such as OpenGL. Descriptions will be added for any subfolders/mini-projects as they are created.

## Environment Setup Instructions
### Linux / Ubuntu 20.04 LTS

- Install CMake on your machine if it is not already installed
- Install GLFW if it is not already installed on the machine
  - Download the source package from https://www.glfw.org/
  - Extract it somewhere on your machine
  - cd into the directory and run `cmake .`
  - Install the GLFW library with `sudo make install`
- Install GLEW if it is not already installed on the machine
  - Download the source package from http://glew.sourceforge.net/index.html
  - Extract it somewhere on your machine
  - cd into the directory and run `sudo make install`
  
With this, you should have the required libraries in case you want to make modifications to the code.

### Windows 10

Environment setup on a Windows machine has yet to be taken care of. When the steps are done, replace this placeholder text with the instructions.

## Tools

GLFW - OpenGL Library

GLEW - OpenGL Extension Wrangler

## Resources

https://learnopengl.com

## Errors
### "undefined reference to `glfwInit()`

Ensure all of the necessary libraries are linked. I had to use `-lglfw3 -lGLEW -lGLU -lGL -lX11 -lpthread -lXrandr -lXi -ld` as command line arguments.

### libGLEW.so.2.1: cannot open shared object file: No such file or directory

This is a dynamic library, so we need to tell the OS where to locate it at runtime. Find the file (by default, mine was installed in /usr/lib64), then check for the existence of the dynamtic library path environment variable, `LD_LIBRARY_PATH`. Add it to your shell configuration (e.g. .bashrc) and export the path to that variable with `export LD_LIBRARY_PATH={location of file}`. Run `source` to load your new shell configurations and run again.