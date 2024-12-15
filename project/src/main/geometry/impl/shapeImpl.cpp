#include "../api/shape.hpp"
#include "../api/shapeBuilder.hpp"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

static float calcWidth(vector<Vertex*> vertices) {
	float max = vertices[0]->getCoordinates().x;
	float min = vertices[0]->getCoordinates().x;
	for (const Vertex* vertex : vertices) {
		const float x = vertex->getCoordinates().x;
		if (x > max)
			max = x;
		else if (x < min)
			min = x;
	}
	return max - min;
}

static float calcHeight(vector<Vertex*> vertices) {
	float max = vertices[0]->getCoordinates().y;
	float min = vertices[0]->getCoordinates().y;
	for (const Vertex* vertex : vertices) {
		const float y = vertex->getCoordinates().y;
		if (y > max)
			max = y;
		else if (y < min)
			min = y;
	}
	return max - min;
}

static float calcDepth(vector<Vertex*> vertices) {
	float max = vertices[0]->getCoordinates().z;
	float min = vertices[0]->getCoordinates().z;
	for (const Vertex* vertex : vertices) {
		const float z = vertex->getCoordinates().z;
		if (z > max)
			max = z;
		else if (z < min)
			min = z;
	}
	return max - min;
}

Shape::Shape(vector<Vertex*> vertices, vector<GLuint> indices, float weight, float value) {
	this->vertices = vertices;
	this->indices = indices;
	this->shapeVAO = 0;
	this->verticesVBO = 0;
	this->colorsVBO = 0;
	this->shapeEBO = 0;
	this->model = mat4(1.0f);
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->width = calcWidth(vertices);
	this->height = calcHeight(vertices);
	this->depth = calcDepth(vertices);
	this->rotationAngle = 0.0f;
	this->weight = weight;
	this->value = value;
	this->anchorWorld = vec4(0.0f);
	this->anchorObj = vec4(0.0f);
	ShapeBuilder::initShape(this);
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

GLuint* Shape::getEBO() {
	return &(this->shapeEBO);
}

vector<Vertex*> Shape::getVertices() const {
	return this->vertices;
}

vector<vec3> Shape::getVerticesCoordinates() const {
	vector<vec3> coordinates;
	for (const Vertex* vertex : this->vertices)
		coordinates.push_back(vertex->getCoordinates());
	return coordinates;
}

vector<vec4> Shape::getVerticesColors() const {
	vector<vec4> colors;
	for (const Vertex* vertex : this->vertices)
		colors.push_back(vertex->getColor());
	return colors;
}

vector<GLuint> Shape::getIndices() const {
	return this->indices;
}

mat4 Shape::getModel() const {
	return this->model;
}

void Shape::setModel(mat4 model) {
	this->model = model;
}

tuple<float, float, float> Shape::getPosition() const {
	return tuple<float, float, float>(this->x, this->y, this->z);
}

float Shape::getWidth() const {
	return this->width;
}

float Shape::getHeight() const {
	return this->height;
}

float Shape::getDepth() const {
	return this->depth;
}

void Shape::move(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;

	vec3 translation = vec3(x, y, z);
	mat4 translationMatrix = translate(mat4(1), translation);

	this->model *= mat4(1.0f) * mat4(1.0f) * translationMatrix;
}

float Shape::getWeight() const {
	return this->weight;
}

float Shape::getValue() const {
	return this->value;
}

float Shape::getRotationAngle() const {
	return this->rotationAngle;
}

void Shape::rotate(float angle) {
	this->rotationAngle += angle;
}