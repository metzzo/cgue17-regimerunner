#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Interpolated values from the vertex shaders
//in vec3 fragmentColor;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main(){
	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	// color = fragmentColor;
	
	// Output color = color of the texture at the specified UV
	color = texture( textureSampler, UV ).rgb;
}