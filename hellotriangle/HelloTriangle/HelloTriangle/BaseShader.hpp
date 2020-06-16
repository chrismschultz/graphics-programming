/*
 * BaseShader.hpp
 * Chris Schultz
 * 16 June 2020
 *
 * Base Shader Class Header
 */

#ifndef BASESHADER_HPP
#define BASESHADER_HPP

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class BaseShader {
public:
	unsigned int ID;

	// Constructor reads and builds the shader from the vertex and fragment paths
	BaseShader(const char* vertexPath, const char* fragmentPath);

	// Activate/Use the shader
	void use();

	// Utility function to set bool
	void setBool(const std::string &name, bool value) const;

	// Utility function to set int
	void setInt(const std::string &name, int value) const;

	// Utility function to set float
	void setFloat(const std::string &name, float value) const;

private:
	// Helper function to check compile and linking status
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif