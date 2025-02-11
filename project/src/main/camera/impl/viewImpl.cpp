#include "../api/view.hpp"

View::View(vec4 position, vec4 target, vec4 upVector) {
	this->position = position;
	this->target = target;
	this->upVector = upVector;
}

vec4 View::getPosition() const {
	return this->position;
}

void View::move(vec4 movement) {
	this->position += movement;
	this->target += movement;
}

vec4 View::getTarget() const {
	return this->target;
}

void View::setTarget(vec3 target) {
	this->target = vec4(target, 0.0f);
}

vec4 View::getUpvector() const {
	return this->upVector;
}

vec4 View::getDirection() const {
	return this->target - this->position;
}

void View::setPosition(vec3 position) {
	this->position = vec4(position, 1.0f);
}