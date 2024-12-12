#pragma once
#include <vector>
#include "vertex.hpp"
#include "shape.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

/// <summary>
/// Class used to create and initialize shapes.
/// </summary>
class ShapeBuilder {
	public:
		/// <summary>
		/// Creates a rectangle.
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="color"></param>
		/// <returns>A vector of vertices that, when connected, create a rectangle.</returns>
		static pair<vector<Vertex*>, vector<GLuint>> createBox(float width, float height, float depth, vec3 color) {
			float x = width / 2.0f;
			float y = height / 2.0f;
			float z = depth / 2.0f;

			vector<Vertex*> vertices;

			// Front
			vertices.push_back(new Vertex(vec3(-x, y, z), color));
			vertices.push_back(new Vertex(vec3(x, y, z), color));
			vertices.push_back(new Vertex(vec3(x, -y, z), color));
			vertices.push_back(new Vertex(vec3(-x, -y, z), color));

			// Back
			vertices.push_back(new Vertex(vec3(-x, y, -z), color));
			vertices.push_back(new Vertex(vec3(x, y, -z), color));
			vertices.push_back(new Vertex(vec3(x, -y, -z), color));
			vertices.push_back(new Vertex(vec3(-x, -y, -z), color));

			vertices.push_back(new Vertex(vec3(0.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f)));

			// Indexes
			vector<GLuint> indices;
			indices.push_back(0); indices.push_back(1); indices.push_back(2);
			indices.push_back(2); indices.push_back(3); indices.push_back(0);
			indices.push_back(1); indices.push_back(5); indices.push_back(6);
			indices.push_back(6); indices.push_back(2); indices.push_back(1);
			indices.push_back(7); indices.push_back(6); indices.push_back(5);
			indices.push_back(5); indices.push_back(4); indices.push_back(7);
			indices.push_back(4); indices.push_back(0); indices.push_back(3);
			indices.push_back(3); indices.push_back(7); indices.push_back(4);
			indices.push_back(4); indices.push_back(5); indices.push_back(1);
			indices.push_back(1); indices.push_back(0); indices.push_back(4);
			indices.push_back(3); indices.push_back(2); indices.push_back(6);
			indices.push_back(6); indices.push_back(7); indices.push_back(3);
			indices.push_back(vertices.size() - 1);

			return { vertices, indices };
		}

		/// <summary>
		/// Initializes a shape's VAO and VBOs.
		/// </summary>
		/// <param name="shape"></param>
		static void initShape(Shape* shape) {
			// Initialize VAO
			glGenVertexArrays(1, shape->getVAO());
			glBindVertexArray(*shape->getVAO());

			// Initialize vertices' VBO
			glGenBuffers(1, shape->getVerticesVBO());
			glBindBuffer(GL_ARRAY_BUFFER, *shape->getVerticesVBO());
			glBufferData(
				GL_ARRAY_BUFFER,
				shape->getVerticesCoordinates().size() * sizeof(vec3),
				shape->getVerticesCoordinates().data(),
				GL_STATIC_DRAW
			);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			// Initialize vertices' colors' VBO
			glGenBuffers(1, shape->getColorsVBO());
			glBindBuffer(GL_ARRAY_BUFFER, *shape->getColorsVBO());
			glBufferData(
				GL_ARRAY_BUFFER,
				shape->getVerticesColors().size() * sizeof(vec4),
				shape->getVerticesColors().data(),
				GL_STATIC_DRAW
			);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(1);

			// Intialize the shape's EBO
			glGenBuffers(1, shape->getEBO());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *shape->getEBO());
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				shape->getIndices().size() * sizeof(GLuint),
				shape->getIndices().data(),
				GL_STATIC_DRAW
			);
		}
};
