#version 330 core

in vec4 fragColor;	// input: color to assign to the fragment
out vec4 color;		// output: same color received from input

void main()
{
	color = fragColor;
}