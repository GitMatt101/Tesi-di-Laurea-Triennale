#include "../api/camera.hpp"
#include "../../utils.hpp"
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera() {
	this->view = new View(
		vec4(2.5f, 5.0f, 15.0f, 1.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 0.0f)
	);
	this->perspective = new Perspective(
		45.0f,
		APP_WIDTH,
		APP_HEIGHT,
		0.1f,
		2000.0f
	);
}

View* Camera::getView() const {
	return this->view;
}

Perspective* Camera::getPerspective() const {
	return this->perspective;
}