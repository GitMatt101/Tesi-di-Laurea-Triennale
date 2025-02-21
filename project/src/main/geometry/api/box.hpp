#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <tuple>
#include "vertex.hpp"
using namespace glm;
using namespace std;

/// <summary>
/// Class that models a box.
/// </summary>
class Box {
	private:
		GLuint VAO;
		GLuint verticesVBO;
		GLuint colorsVBO;
		GLuint EBO;
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
		Box(vector<Vertex*> vertices, vector<GLuint> indices, int id, float value, float weight);

		int getId() const;

		/// <returns>The pointer to the box's VAO.</returns>
		GLuint* getVAO();

		/// <returns>The pointer to the box's vertices' VBO.</returns>
		GLuint* getVerticesVBO();

		/// <returns>The pointer to the box's vertices' colors' VBO.</returns>
		GLuint* getColorsVBO();

		/// <returns>The pointer to the box's EBO.</returns>
		GLuint* getEBO();

		/// <returns>The box's vertices.</returns>
		vector<Vertex*> getVertices() const;

		/// <returns>The box's vertices' coordinates</returns>
		vector<vec3> getVerticesCoordinates() const;

		/// <returns>The box's vertices' colors</returns>
		vector<vec4> getVerticesColors() const;

		vector<GLuint> getIndices() const;

		void setIndices(vector<GLuint> indices);

		/// <returns>The box's model matrix.</returns>
		mat4 getModel() const;

		/// <summary>
		/// Sets the model matrix of the box.
		/// </summary>
		/// <param name="model">- The new model matrix.</param>
		void setModel(mat4 model);

		/// <returns>The box's position as a vec3</returns>
		vec3 getPosition() const;

		/// <summary>
		/// Sets the box's position.
		/// </summary>
		/// <param name="position">New position.</param>
		void setPosition(vec3 position);

		/// <summary>
		/// Sets the box's position to its starting coordinates.
		/// </summary>
		void restartPosition();

		/// <returns>The box's size as a vec3.</returns>
		vec3 getSize() const;

		/// <summary>
		/// Moves the box.
		/// </summary>
		/// <param name="x">- Horizontal movement.</param>
		/// <param name="y">- Vertical movement.</param>
		/// <param name="z">- Depth movement.</param>
		void move(float x, float y, float z);

		/// <returns>The box's weight.</returns>
		float getWeight() const;

		/// <returns>The box's value.</returns>
		float getValue() const;

		/// <summary>
		/// Checks if the box has arrived to its target coordinates.
		/// </summary>
		bool targetReached() const;

		/// <summary>
		/// Sets the target coordinates for the box's movement
		/// </summary>
		/// <param name="target"></param>
		void setTarget(vec3 target);

		/// <summary>
		/// Moves towards the target.
		/// </summary>
		void moveTowardsTarget();

		/// <summary>
		/// Checks if the box has arrived to its starting position.
		/// </summary>
		bool startReached() const;

		/// <summary>
		/// Moves towards the starting position.
		/// </summary>
		void moveTowardsStart();

		/// <summary>
		/// Inits VAO and VBOs.
		/// </summary>
		void init();

		/// <summary>
		/// Sets the starting position, also setting current position.
		/// </summary>
		/// <param name="position"></param>
		void setStartPosition(vec3 position);
};