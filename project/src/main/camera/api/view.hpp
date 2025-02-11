#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;

class View {
	private:
		vec4 position;
		vec4 target;
		vec4 upVector;

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		/// <param name="position">- Initial position of the camera.</param>
		/// <param name="target">- Point the camera is pointing at.</param>
		/// <param name="upVector"></param>
		View(vec4 position, vec4 target, vec4 upVector);

		/// <returns>The position of the camera.</returns>
		vec4 getPosition() const;

		/// <summary>
		/// Sets the camera position.
		/// </summary>
		/// <param name="position"></param>
		void setPosition(vec3 position);

		/// <summary>
		/// Moves the view position and target.
		/// </summary>
		/// <param name="movement">- Movement vector.</param>
		void move(vec4 movement);

		/// <returns>The position of the camera.</returns>
		vec4 getTarget() const;

		/// <summary>
		/// Sets the target of the camera.
		/// </summary>
		/// <param name="target"></param>
		void setTarget(vec3 target);

		/// <returns>The position of the camera.</returns>
		vec4 getUpvector() const;

		/// <returns>The direction of the camera</returns>
		vec4 getDirection() const;
};