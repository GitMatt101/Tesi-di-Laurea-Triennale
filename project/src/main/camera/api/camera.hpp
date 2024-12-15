#pragma once
#include "../api/perspective.hpp"
#include "../api/view.hpp"

class Camera {
	private:
		View* view;
		Perspective* perspective;

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		Camera();

		/// <returns>The view of the camera.</returns>
		View* getView() const;

		/// <returns>The perspective of the view.</returns>
		Perspective* getPerspective() const;
};