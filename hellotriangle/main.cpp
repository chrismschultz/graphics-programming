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
 * TEMPORARY SHADER SOURCE CODE
 */
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

/*
 * FUNCTION PROTOTYPES
 */

/* Callback function that gets called each time the window is resized. */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/* Checking for user input */
void processInput(GLFWwindow* window);

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

    /* Create vertex data containing three vertices for a triangle */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    /* Declare a Vertex Buffer Object to store the vertices in GPU memory */
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Create a shader object, storing its ID as an unsigned int */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    /* Set the shader source and compile it. FIrst argument is the shader object
     * to compile. Second specifies how many strings we are passing as source
     * code. Third is the actual source code. Fourth is an array of string
     * lengths - leave as NULL */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /* Test for successful shader compilation */
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


    /* Tell OpenGL the size of the rendering window so that OpenGL knows how to display
     * the data and coordinates with respect to the window. 
     * First two parameters set the location of the lower left corner of the window. 
     * The third and fourth set the width and height of the rendering window in pixels. */
    glViewport(0, 0, 800, 600);

    /* Create a render loop. If the window hasn't been instructed to close, swap the color
     * buffer (update the color values for each pixel in the window) and poll to see if
     * any events like keyboard or mouse events havebeen triggered */
    while (!glfwWindowShouldClose(window)){
        
        /* Test for user input */
        processInput(window);

        /* Execute rendering commands */
        glClearColor(0.255f, 0.588f, 0.882f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Check and call events and swap the buffers */
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

/* Checking for user input */
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}