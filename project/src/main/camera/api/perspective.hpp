#pragma once

class Perspective {
	private:
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;

	public:
		Perspective(float fov, float width, float height, float nearPlane, float farPlane);

		/// <returns>Field of View.</returns>
		float getFOV() const;

		/// <returns>The camera's aspect ratio.</returns>
		float getAspectRatio() const;

		/// <returns>The camera's near plane.</returns>
		float getNearPlane() const;

		/// <returns>The camera's far plane.</returns>
		float getFarPlane() const;

		/// <summary>
		/// Zooms in or out.
		/// </summary>
		/// <param name="zoom">- Zoom value (positive to zoom in, negative to zoom out).</param>
		void zoom(float zoom);
};