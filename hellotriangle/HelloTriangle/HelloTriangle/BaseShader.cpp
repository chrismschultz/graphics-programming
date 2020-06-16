/*
 * BaseShader.cpp
 * Chris Schultz
 * 16 June 2020
 *
 * Base Shader Class Definitions
 */

#include "BaseShader.hpp"

// Constructor reads and builds the shader from the vertex and fragment paths
BaseShader::BaseShader(const char* vertexPath, const char* fragmentPath) {
	
	/* ----- Retrieve the vertex/fragment source code from the paths ----- */

	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;

	// Ensure the ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	// Try to open and read the files
	try {
		// Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read the file's buffer contents into the streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close the file handlers
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Error: shader file unsuccessfully found" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	/* ----- Compile the shaders ----- */

	unsigned int vertex, fragment;
	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// Delete the unnecessary shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Activate/Use the shader
void BaseShader::use() {
	glUseProgram(ID);
}

// Utility function to set bool
void BaseShader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

// Utility function to set int
void BaseShader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// Utility function to set float
void BaseShader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// Helper function to check compile and linking status
void::BaseShader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Error: Shader compilation error\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Error: program linking error\n" << infoLog << std::endl;
		}
	}
}