#pragma once
#include <vector>
#include "vertex.hpp"
#include "box.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

/// <summary>
/// Class used to create and initialize GL objects.
/// </summary>
class ShapeBuilder {
	public:
		/// <summary>
		/// Creates a box.
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="color"></param>
		/// <returns>A vector of vertices that, when connected, create a rectangle.</returns>
		static pair<vector<Vertex*>, vector<GLuint>> createBox(float width, float height, float depth, vec4 color);

		/// <summary>
		/// Initializes a shape's VAO and VBOs.
		/// </summary>
		/// <param name="shape"></param>
		static void initShape(Box* shape);
};
