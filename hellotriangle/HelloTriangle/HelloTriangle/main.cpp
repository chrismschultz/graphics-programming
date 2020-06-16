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

const char *fragmentShaderOne = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(){\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

const char *fragmentShaderTwo = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(){\n"
		"FragColor = vec4(0.965f, 0.969f, 0.2f, 0.251f);\n"
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

	// Create and compile the first fragment shader, which outputs an orange color
	unsigned int fShaderOne, fShaderTwo;
	fShaderOne = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderOne, 1, &fragmentShaderOne, NULL);
	glCompileShader(fShaderOne);
	glGetShaderiv(fShaderOne, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShaderOne, 512, NULL, infoLog);
		std::cout << "Error: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
	}

	// Create and compile the second fragment shader, which outputs a yellow color
	fShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderTwo, 1, &fragmentShaderTwo, NULL);
	glCompileShader(fShaderTwo);
	glGetShaderiv(fShaderTwo, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShaderOne, 512, NULL, infoLog);
		std::cout << "Error: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
	}

	// Create and link the shader program
	unsigned int shaderProgramOne, shaderProgramTwo;
	shaderProgramOne = glCreateProgram();
	glAttachShader(shaderProgramOne, vertexShader);
	glAttachShader(shaderProgramOne, fShaderOne);
	glLinkProgram(shaderProgramOne);

	// Test for successful shader program linking
	glGetProgramiv(shaderProgramOne, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramOne, 512, NULL, infoLog);
		std::cout << "Error: Shader Program Linking Failed\n" << infoLog << std::endl;
	}

	// Create and link the second shader program
	shaderProgramTwo = glCreateProgram();
	glAttachShader(shaderProgramTwo, vertexShader);
	glAttachShader(shaderProgramTwo, fShaderTwo);
	glLinkProgram(shaderProgramTwo);

	// Test for successful shader program linking
	glGetProgramiv(shaderProgramTwo, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramTwo, 512, NULL, infoLog);
		std::cout << "Error: Shader Program Linking Failed\n" << infoLog << std::endl;
	}

	// Delete the now unused shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fShaderOne);
	glDeleteShader(fShaderTwo);

	/* Set up vertex data and configure vertex attributes 
	 * -------------------------------------------------- */

	// Create vertex data containing three vertices for a triangle
	float triangleOneVertices[] = {
		-0.5f, -0.5f, 0.0f, // lower left
		-0.25f, 0.5f, 0.0f, // upper
		0.0f, -0.5f, 0.0f, // lower right
	};

	float triangleTwoVertices[] = {
		0.0f, -0.5f, 0.0f, // lower left
		0.25f, 0.5f, 0.0f, // upper
		0.5f, -0.5f, 0.0f, // lower right
	};

	// Create index data containing the the position of each vertex in vertices to draw
	unsigned int triangleIndices[] = {
		0, 1, 2,
	};

	// Declare a Vertex Buffer Object to store the vertices in GPU memory
	unsigned int vbos[2], vaos[2], ebos[2];
	glGenVertexArrays(2, vaos);
	glGenBuffers(2, vbos);
	glGenBuffers(2, ebos);

	// Set up the objects for the first triangle
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOneVertices), triangleOneVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up the objects for the second triangle
	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwoVertices), triangleTwoVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
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

		// Draw the first triangle
		glUseProgram(shaderProgramOne);
		glBindVertexArray(vaos[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// Draw the second triangle
		glUseProgram(shaderProgramTwo);
		glBindVertexArray(vaos[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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

// Callback function that gets called each time the window is resized */
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
}

// Checking for user input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}