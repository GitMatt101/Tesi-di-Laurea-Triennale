#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <tuple>
#include "vertex.hpp"
using namespace glm;
using namespace std;

/// <summary>
/// Class that models a generic shape.
/// </summary>
class Shape {
	private:
		GLuint shapeVAO;
		GLuint verticesVBO;
		GLuint colorsVBO;
		GLuint shapeEBO;
		mat4 model;
		vector<Vertex*> vertices;
		vector<GLuint> indices;
		float x;
		float y;
		float z;
		float width;
		float height;
		float depth;
		float rotationAngle;
		float value;
		float weight;
		vec4 anchorWorld;
		vec4 anchorObj;

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		/// <param name="vertices"></param>
		Shape(vector<Vertex*> vertices, vector<GLuint> indices, float value, float weight);

		/// <returns>The pointer to the shape's VAO.</returns>
		GLuint* getVAO();

		/// <returns>The pointer to the shape's vertices' VBO.</returns>
		GLuint* getVerticesVBO();

		/// <returns>The pointer to the shape's vertices' colors' VBO.</returns>
		GLuint* getColorsVBO();

		/// <returns>The pointer to the shape's EBO.</returns>
		GLuint* getEBO();

		/// <returns>The shape's vertices.</returns>
		vector<Vertex*> getVertices() const;

		/// <returns>The shape's vertices' coordinates</returns>
		vector<vec3> getVerticesCoordinates() const;

		/// <returns>The shape's vertices' colors</returns>
		vector<vec4> getVerticesColors() const;

		vector<GLuint> getIndices() const;

		/// <returns>The shape's model matrix.</returns>
		mat4 getModel() const;

		/// <summary>
		/// Sets the model matrix of the shape.
		/// </summary>
		/// <param name="model">- The new model matrix.</param>
		void setModel(mat4 model);

		/// <returns>The shape's position as a pair. The first value is x, the second is y and the third is z.</returns>
		tuple<float, float, float> getPosition() const;

		/// <returns>The shape's width.</returns>
		float getWidth() const;

		/// <returns>The shape's height.</returns>
		float getHeight() const;

		/// <returns>The shape's depth.</returns>
		float getDepth() const;

		/// <summary>
		/// Moves the shape.
		/// </summary>
		/// <param name="x">- Horizontal movement.</param>
		/// <param name="y">- Vertical movement.</param>
		/// <param name="z">- Depth movement.</param>
		void move(float x, float y, float z);

		/// <returns>The shape's weight.</returns>
		float getWeight() const;

		/// <returns>The shape's value.</returns>
		float getValue() const;

		/// <returns>The shape's rotation angle in degrees.</returns>
		float getRotationAngle() const;

		/// <summary>
		/// Rotates the shape by increasing its rotation angle.
		/// </summary>
		/// <param name="angle">- The rotation angle to be added to the current one.</param>
		void rotate(float angle);
};