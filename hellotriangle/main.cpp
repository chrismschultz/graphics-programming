/*
 * main.cpp
 * Chris Schultz
 * 30 May 2020
 *
 * "Hello Triangle" introductory program
 */

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
using std::cout;
using std::endl;

/*
 * FUNCTION PROTOTYPES
 */

/* Callback function that gets called each time the window is resized. */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/*
 * MAIN BODY
 */

int main(){

    /* Configures the GLFW library. First argument specifies the option
     * to configure, and the second sets its value. Here, we are specifying
     * GLFW version 3.3. Saying we want to use the core profile means we
     * do not use backwards-compatible features we don't need. */
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a window object. First two arguments are its width and height. Third
     * is the name for the window. Fourth argument is the GLFWMonitor to use for 
     * fullscreen mode - setting this to NULL means it is run in windowed mode. Last
     * is the GLFWwindow to share resources with - setting this to NULL means that it
     * does not share resources */
    GLFWwindow* window = glfwCreateWindow(800, 600, "First OpenGL Project", NULL, NULL);
    if (window == NULL){
        cout << "Failed to create a window!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    /* Initialize GLEW to manage OpenGL function pointers */
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err){
        cout << "Error initializing GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    /* Tell OpenGL the size of the rendering window so that OpenGL knows how to display
     * the data and coordinates with respect to the window. 
     * First two parameters set the location of the lower left corner of the window. 
     * The third and fourth set the width and height of the rendering window in pixels. */
    glViewport(0, 0, 800, 600);

    /* Create a render loop. If the window hasn't been instructed to close, swap the color
     * buffer (update the color values for each pixel in the window) and poll to see if
     * any events like keyboard or mouse events havebeen triggered */
    while (!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Terminate the window, cleaning all of GLFW's allocated resources */
    glfwTerminate();
    return 0;
}

/*
 * FUNCTION DEFINITIONS
 */

/* Callback function that gets called each time the window is resized */
void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}