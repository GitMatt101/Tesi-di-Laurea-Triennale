#include "../api/box.hpp"
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

Box::Box(vector<Vertex*> vertices, vector<GLuint> indices, int id, float weight, float value) {
	this->id = id;
	this->vertices = vertices;
	this->indices = indices;
	this->VAO = 0;
	this->verticesVBO = 0;
	this->colorsVBO = 0;
	this->EBO = 0;
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

int Box::getId() const {
	return this->id;
}

void Box::init() {
	ShapeBuilder::initShape(this);
}

GLuint* Box::getVAO() {
	return &(this->VAO);
}

GLuint* Box::getVerticesVBO() {
	return &(this->verticesVBO);
}

GLuint* Box::getColorsVBO() {
	return &(this->colorsVBO);
}

GLuint* Box::getEBO() {
	return &(this->EBO);
}

vector<Vertex*> Box::getVertices() const {
	return this->vertices;
}

vector<vec3> Box::getVerticesCoordinates() const {
	vector<vec3> coordinates;
	for (const Vertex* vertex : this->vertices)
		coordinates.push_back(vertex->getCoordinates());
	return coordinates;
}

vector<vec4> Box::getVerticesColors() const {
	vector<vec4> colors;
	for (const Vertex* vertex : this->vertices)
		colors.push_back(vertex->getColor());
	return colors;
}

vector<GLuint> Box::getIndices() const {
	return this->indices;
}

mat4 Box::getModel() const {
	return this->model;
}

void Box::setModel(mat4 model) {
	this->model = model;
}

vec3 Box::getPosition() const {
	return vec3(this->x, this->y, this->z);
}

vec3 Box::getSize() const {
	return vec3(this->width, this->height, this->depth);
}

void Box::move(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;

	vec3 translation = vec3(x, y, z);
	mat4 translationMatrix = translate(mat4(1), translation);

	this->model *= mat4(1.0f) * mat4(1.0f) * translationMatrix;
}

void Box::setPosition(vec3 position) {
	vec3 translation = position - this->getPosition();
	mat4 translationMatrix = translate(mat4(1), translation);

	this->x = position.x;
	this->y = position.y;
	this->z = position.z;

	this->model *= mat4(1.0f) * mat4(1.0f) * translationMatrix;
}

float Box::getWeight() const {
	return this->weight;
}

float Box::getValue() const {
	return this->value;
}

bool Box::targetReached() const {
	return abs(this->xTarget - this->x) <= 0.001 && abs(this->yTarget - this->y) <= 0.001 && abs(this->zTarget - this->z) <= 0.001;
}

void Box::moveTowardsTarget() {
	float xStep = (this->xTarget - this->xStart) / (FRAME_LENGTH * 3);
	float yStep = (this->yTarget - this->yStart) / (FRAME_LENGTH * 3);
	float zStep = (this->zTarget - this->zStart) / (FRAME_LENGTH * 3);
	this->move(xStep, yStep, zStep);
}

bool Box::startReached() const {
	return abs(this->xStart - this->x) <= 0.001 && abs(this->yStart - this->y) <= 0.001 && abs(this->zStart - this->z) <= 0.001;
}

void Box::moveTowardsStart() {
	float xStep = (this->xStart - this->xTarget) / (FRAME_LENGTH * 3);
	float yStep = (this->yStart - this->yTarget) / (FRAME_LENGTH * 3);
	float zStep = (this->zStart - this->zTarget) / (FRAME_LENGTH * 3);
	this->move(xStep, yStep, zStep);
}

void Box::setIndices(vector<GLuint> indices) {
	this->indices = indices;
}

void Box::setTarget(vec3 target) {
	this->xTarget = target.x;
	this->yTarget = target.y;
	this->zTarget = target.z;
}

void Box::restartPosition() {
	this->setPosition(vec3(this->xStart, this->yStart, this->zStart));
}

void Box::setStartPosition(vec3 position) {
	this->xStart = position.x;
	this->yStart = position.y;
	this->zStart = position.z;

	this->setPosition(position);
}