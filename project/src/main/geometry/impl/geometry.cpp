#include "../api/geometry.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

vector<Vertex> createCircle(float rx, float ry, int precision, vec4 color) {
	float step = 2 * (float)M_PI / precision;
	vector<Vertex> vertices;
	vertices.push_back({ vec3(0.0f), color });
	for (int i = 0; i <= precision + 2; i++) {
		float angle = (float)i * step;
		vertices.push_back({
			vec3(rx * cos(angle), ry * sin(angle), 0.0f),
			color
		});
	}
	return vertices;
}

vector<Vertex> createRectangle(float width, float height, vec4 color) {
	vector<Vertex> vertices;
	// Top Left
	vertices.push_back({
		vec3(-width / 2.0f, height / 2.0f, 0.0f),
		color
	});
	// Top Right
	vertices.push_back({
		vec3(width / 2.0f, height / 2.0f, 0.0f),
		color
	});
	// Bottom Right
	vertices.push_back({
		vec3(width / 2.0f, -height / 2.0f, 0.0f),
		color
	});
	// Bottom Left
	vertices.push_back({
		vec3(-width / 2.0f, -height / 2.0f, 0.0f),
		color
	});
	return vertices;
}

vector<Vertex> createArrow(float length, float thickness, vec4 color) {
	//						    C
	//						  ##
	// A					B #####
	// ###############################			|
	// ################################# D		|	thickness
	// ###############################			|
	// G					F #####
	//						  ##
	//						    E
	// 
	// ---------------------------------
	//				length

	vector<Vertex> vertices;
	// Top Left (A)
	vertices.push_back({
		vec3(-length / 2.0f, thickness / 2.0f, 0.0f),
		color
	});
	// End fo stick, start of arrowhead top (B)
	vertices.push_back({
		vec3(length / 2.0f - thickness, thickness / 2.0f, 0.0f),
		color
	});
	// Top vertex of arrowhead (C)
	vertices.push_back({
		vec3(length / 2.0f - thickness, thickness, 0.0f),
		color
	});
	// Right vertex of arrowhead (D)
	vertices.push_back({
		vec3(length / 2.0f, 0.0f, 0.0f),
		color
	});
	// Bottom vertex of arrowhead (E)
	vertices.push_back({
		vec3(length / 2.0f - thickness, -thickness, 0.0f),
		color
	});
	// End of stick, start of arrowhead bototm (F)
	vertices.push_back({
		vec3(length / 2.0f - thickness, -thickness / 2.0f, 0.0f),
		color
	});
	// Bottom Left (G)
	vertices.push_back({
		vec3(-length / 2.0f, -thickness / 2.0f, 0.0f),
		color
	});
	return vertices;
}