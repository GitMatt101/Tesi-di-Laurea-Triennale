#version 420 core

// Input variables
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;

// Transformation matrices
uniform mat4 projection;
uniform mat4 model;

// Output variable that is sent to the fragment shader
out vec4 color;

void main()
{
	// Determines the position of the vertex using the transformation matrices
	glPosition = projection * model * vec4(vertexPosition, 1.0f);

	// Passes the color to the fragment shader to assign it to the corresponding vertex
	color = vertexColor;
} 