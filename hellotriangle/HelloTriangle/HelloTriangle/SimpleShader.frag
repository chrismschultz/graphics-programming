#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D metalTexture;
uniform sampler2D happyTexture;
uniform float textureMix;

void main(){
	FragColor = mix(texture(metalTexture, texCoord), texture(happyTexture, texCoord), textureMix);
}