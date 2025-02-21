#include "../api/perspective.hpp"
#include <iostream>

Perspective::Perspective(float fov, float width, float height, float nearPlane, float farPlane) {
	this->fov = fov;
	this->aspectRatio = width/height;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

float Perspective::getFOV() const {
	return this->fov;
}

float Perspective::getAspectRatio() const {
	return this->aspectRatio;
}

float Perspective::getNearPlane() const {
	return this->nearPlane;
}

float Perspective::getFarPlane() const {
	return this->farPlane;
}

void Perspective::zoom(float zoom) {
	this->fov += zoom;

	if (this->fov < 1.0f)
		this->fov = 1.0f;
	if (this->fov > 120.0f)
		this->fov = 120.0f;
}