#pragma once
#include "../../lib.hpp"

/**
* Structure used for shapes' vertices.
*
* @param coordinates - position of the vertex.
* @param color - color of the vertex.
*/
struct Vertex {
	vec3 coordinates;
	vec4 color;

	bool operator==(const Vertex& other) const
	{
		return coordinates == other.coordinates && color == other.color;
	}
};

/**
* Creates a circle.
*
* @param rx - horizontal radius of the circle.
* @param ry - vertical radius of the circle.
* @param precision - number of vertices to create.
* @param color - color of the circle.
* @return A vector of vertices that form a circle.
*/
vector<Vertex> createCircle(float rx, float ry, int precision, vec4 color);

/**
* Creates a rectangle.
*
* @param width - width of the rectangle.
* @param height - height of the rectangle.
* @param color1 - color of the rectangle.
* @return A vector of vertices that form a rectangle.
*/
vector<Vertex> createRectangle(float width, float height, vec4 color);

/**
* Creates an arrow.
*
* @param length - length of the arrow.
* @param thickness - thickness of the arrow.
* @param color - color of the arrow.
* @return A vector of vertices that form an arrow.
*/
vector<Vertex> createArrow(float length, float thickness, vec4 color);