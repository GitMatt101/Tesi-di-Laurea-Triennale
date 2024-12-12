#pragma once
#include <glm/vec3.hpp>
using namespace glm;

/// <summary>
/// Structure of a vertex.
/// </summary>
class Vertex {
	private:
		vec3 coordinates;
		vec3 color;

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		/// <param name="coordinates"></param>
		/// <param name="color"></param>
		Vertex(vec3 coordinates, vec3 color);

		/// <returns>The vertex' coordinates.</returns>
		vec3 getCoordinates() const;

		/// <summary>
		/// Moves the vertex.
		/// </summary>
		/// <param name="x">- Horizontal movement.</param>
		/// <param name="y">- Vertical movement.</param>
		void move(float x, float y);

		/// <summary>
		/// Sets the vertex' coordinates.
		/// </summary>
		/// <param name="coordinates"></param>
		void setCoordinates(vec3 coordinates);

		/// <returns>The vertex' color.</returns>
		vec3 getColor() const;
};