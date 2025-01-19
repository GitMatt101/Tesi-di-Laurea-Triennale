#include "../api/shape.hpp"
#include "../api/shapeBuilder.hpp"
#include "../../utils.hpp"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <tuple>

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

Shape::Shape(vector<Vertex*> vertices, vector<GLuint> indices, int id, float weight, float value) {
	this->id = id;
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
	this->xTarget = 0.0f;
	this->yTarget = 0.0f;
	this->zTarget = 0.0f;
	this->xStart = 0.0f;
	this->yStart = 0.0f;
	this->zStart = 0.0f;
	this->width = calcWidth(vertices);
	this->height = calcHeight(vertices);
	this->depth = calcDepth(vertices);
	this->weight = weight;
	this->value = value;
	this->anchorWorld = vec4(0.0f);
	this->anchorObj = vec4(0.0f);
}

int Shape::getId() const {
	return this->id;
}

void Shape::init() {
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

vec3 Shape::getPosition() const {
	return vec3(this->x, this->y, this->z);
}

vec3 Shape::getSize() const {
	return vec3(this->width, this->height, this->depth);
}

void Shape::move(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;

	vec3 translation = vec3(x, y, z);
	mat4 translationMatrix = translate(mat4(1), translation);

	this->model *= mat4(1.0f) * mat4(1.0f) * translationMatrix;
}

void Shape::setPosition(vec3 position) {
	vec3 translation = position - this->getPosition();
	mat4 translationMatrix = translate(mat4(1), translation);

	this->x = position.x;
	this->y = position.y;
	this->z = position.z;

	this->model *= mat4(1.0f) * mat4(1.0f) * translationMatrix;
}

float Shape::getWeight() const {
	return this->weight;
}

float Shape::getValue() const {
	return this->value;
}

bool Shape::targetReached() const {
	return abs(this->xTarget - this->x) <= 0.001 && abs(this->yTarget - this->y) <= 0.001 && abs(this->zTarget - this->z) <= 0.001;
}

void Shape::moveTowardsTarget() {
	float xStep = (this->xTarget - this->xStart) / (FRAME_LENGTH * 3);
	float yStep = (this->yTarget - this->yStart) / (FRAME_LENGTH * 3);
	float zStep = (this->zTarget - this->zStart) / (FRAME_LENGTH * 3);
	this->move(xStep, yStep, zStep);
}

void Shape::setIndices(vector<GLuint> indices) {
	this->indices = indices;
}

void Shape::setTarget(vec3 target) {
	this->xTarget = target.x;
	this->yTarget = target.y;
	this->zTarget = target.z;
}

float Shape::getVolume() const {
	return this->width * this->height * this->depth;
}

void Shape::restartPosition() {
	this->setPosition(vec3(this->xStart, this->yStart, this->zStart));
}

void Shape::setStartPosition(vec3 position) {
	this->xStart = position.x;
	this->yStart = position.y;
	this->zStart = position.z;

	this->setPosition(position);
}