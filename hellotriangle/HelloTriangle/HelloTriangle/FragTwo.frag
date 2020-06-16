#version 330 core

out vec4 FragColor;

in vec3 vertPosition;

void main() {
	FragColor = vec4(vertPosition, 1.0);
}