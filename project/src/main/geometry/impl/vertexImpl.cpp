#include "../api/vertex.hpp"

Vertex::Vertex(vec3 coordinates, vec4 color) {
	this->coordinates = coordinates;
	this->color = color;
}

vec3 Vertex::getCoordinates() const {
	return this->coordinates;
}

void Vertex::setCoordinates(vec3 coordinates) {
	this->coordinates = coordinates;
}

vec4 Vertex::getColor() const {
	return this->color;
}
