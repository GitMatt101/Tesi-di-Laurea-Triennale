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
		int id;
		float x;
		float y;
		float z;
		float xTarget;
		float yTarget;
		float zTarget;
		float xStart;
		float yStart;
		float zStart;
		float width;
		float height;
		float depth;
		float value;
		float weight;
		vec4 anchorWorld;
		vec4 anchorObj;

	public:
		/// <summary>
		/// Base constructior
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		/// <param name="value"></param>
		/// <param name="weight"></param>
		/// <param name="xTarget"></param>
		/// <param name="yTarget"></param>
		/// <param name="zTarget"></param>
		Shape(vector<Vertex*> vertices, vector<GLuint> indices, int id, float value, float weight);

		int getId() const;

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

		void setIndices(vector<GLuint> indices);

		/// <returns>The shape's model matrix.</returns>
		mat4 getModel() const;

		/// <summary>
		/// Sets the model matrix of the shape.
		/// </summary>
		/// <param name="model">- The new model matrix.</param>
		void setModel(mat4 model);

		/// <returns>The shape's position as a vec3</returns>
		vec3 getPosition() const;

		void setPosition(vec3 position);

		void restartPosition();

		/// <returns>The shape's size as a vec3.</returns>
		vec3 getSize() const;

		float getVolume() const;

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

		bool targetReached() const;

		void setTarget(vec3 target);

		void moveTowardsTarget();

		void init();

		void setStartPosition(vec3 position);
};