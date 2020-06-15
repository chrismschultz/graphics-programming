/*
 * main.cpp 
 * Chris Schultz
 * 15 June 2020
 *
 * "Hello Triangle" introductory program
 */

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
 * FUNCTION PROTOTYPES
 */

/* Callback function that gets called each time the window is resized */
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

/* Checking for user input */
void processInput(GLFWwindow* window);

/*
 * TEMPORARY GLOBAL VARIABLES
 */

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main(){\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

/*
 * MAIN BODY
 */

int main() {
	GLFWwindow * window;
	GLenum err;
	int success;
	char infoLog[512];

	/* Create the GLFW window 
	 * ---------------------- */

	// Configure the GLFW library
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window object
	window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	/* Initialize GLEW to manage OpenGL function pointers 
	 * -------------------------------------------------- */

	glewExperimental = GL_TRUE;
	err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error initializing GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Build and compile the shader program
	 * ------------------------------------ */
	 
	// Create and compile the vertex shader, which processes the individual vertices
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Test for successful shader compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error: Shader Vertex Compilation Failed\n" << infoLog << std::endl;
	}

	// Create and compile the fragment shader, which calculates the color output of the pixels
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Test for successful shader compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
	}

	// Create and link the shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Test for successful shader program linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error: Shader Program Linking Failed\n" << infoLog << std::endl;
	}

	// Delete the now unused shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Set up vertex data and configure vertex attributes 
	 * -------------------------------------------------- */

	// Create vertex data containing three vertices for a triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// Declare a Vertex Buffer Object to store the vertices in GPU memory
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Tell OpenGL the size of the rendering window
	glViewport(0, 0, 800, 600);

	/* Render loop 
	 * ----------- */
	while (!glfwWindowShouldClose(window)) {

		// Test for user input
		processInput(window);

		// Execute rendering commands
		glClearColor(0.255f, 0.588f, 0.882f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the shape to the screen
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate the window, cleaning all of GLFW's allocated resources
	glfwTerminate();
	return 0;
}

/*
 * FUNCTION DEFINITIONS
 */

/* Callback function that gets called each time the window is resized */
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
}

/* Checking for user input */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}