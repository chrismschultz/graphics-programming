/*
 * main.cpp 
 * Chris Schultz
 * 15 June 2020
 *
 * "Hello Triangle" introductory program
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "BaseShader.hpp"

/*
 * FUNCTION PROTOTYPES
 */

/* Callback function that gets called each time the window is resized */
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

/* Checking for user input */
void processInput(GLFWwindow* window, float &mixValue);

/*
 * TEMPORARY GLOBAL VARIABLES
 */

/*
 * MAIN BODY
 */

int main() {
	GLFWwindow * window;
	GLenum err;
	float mixValue = 0.2f;

	/* ----- Create the GLFW Window ----- */

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

	/* ----- Initialize GLEW to manage OpenGL function pointers ----- */

	glewExperimental = GL_TRUE;
	err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error initializing GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* ----- Build and compile the shader program ----- */
	 
	BaseShader ShaderOne("SimpleShader.vert", "SimpleShader.frag");

	/* ----- Set up vertex data and configure attributes ----- */

	// Create vertex data containing information to draw a rectangle
	float vertices[] = {
		// positions          // colors            // texture coords
		 0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f,    2.0f, 2.0f,    // top right
		 0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f,    2.0f, 0.0f,    // bottom right
		-0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    // bottom left
		-0.5f,  0.5f,  0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 2.0f     // top left
	};

	// Create index data containing the the position of each vertex in vertices to draw
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Declare and generate buffer objects
	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Set up the objects for the first triangle
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set up position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set up texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* ----- Set up texture attributes ----- */

	unsigned int texture, texture2;
	stbi_set_flip_vertically_on_load(true);

	// Generate, bind, and load first texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Load the image usisng the stb_image functions
	int width, height, numChannels;
	unsigned char *data = stbi_load("metal.jpg", &width, &height, &numChannels, 0);

	if (data) {
		// @params Texture target, mipmap level, texture storage format, width, height, always 0,
		// format, data type, and image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error: Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Generate, bind, and load second texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Load the image
	data = stbi_load("happy.png", &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error: failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ShaderOne.use();

	ShaderOne.setInt("metalTexture", 0);
	ShaderOne.setInt("happyTexture", 1);

	// Tell OpenGL the size of the rendering window
	glViewport(0, 0, 800, 600);

	/* ----- Render loop ----- */
	while (!glfwWindowShouldClose(window)) {

		// Test for user input
		processInput(window, mixValue);

		// Execute rendering commands
		glClearColor(0.255f, 0.588f, 0.882f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind the textures to the appropriate units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ShaderOne.setFloat("textureMix", mixValue);

		// Draw the first triangle
		ShaderOne.use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
void processInput(GLFWwindow* window, float &mixValue) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.001f;
		if (mixValue >= 1.0f) {
			mixValue = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.001f;
		if (mixValue <= 0.0f) {
			mixValue = 0.0f;
		}
	}
}