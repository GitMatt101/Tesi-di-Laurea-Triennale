#include "../api/shapes.hpp"
#include "../../vao/api/vaoInitializer.hpp"

Shape::Shape(vector<Vertex> vertices) {
	this->vertices = vertices;
	this->shapeVAO = 0;
	this->verticesVBO = 0;
	this->colorsVBO = 0;
	this->model = mat4(1.0f);
	this->x = 0.0f;
	this->y = 0.0f;
	this->rotationAngle = 0.0f;
}

void Shape::initVAO() {
	initShapeVAO(this);
}

GLuint* Shape::getVAO() {
	return &(this->shapeVAO);
}

GLuint* Shape::getVerticesVBO() {
	return &(this->verticesVBO);
}

GLuint* Shape::getColorsVBO() {
	return &(this->colorsVBO);
}

vector<Vertex> Shape::getVertices() {
	return this->vertices;
}

mat4 Shape::getModel() {
	return this->model;
}

void Shape::setModel(mat4 model) {
	this->model = model;
}

pair<float, float> Shape::getPosition() {
	return { this->x, this->y };
}

void Shape::move(float x, float y) {
	this->x += x;
	this->y += y;
}

float Shape::getRotationAngle() {
	return this->rotationAngle;
}

void Shape::rotate(float angle) {
	this->rotationAngle += angle;
}