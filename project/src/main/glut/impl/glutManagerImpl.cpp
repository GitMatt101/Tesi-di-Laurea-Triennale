#include "../api/glutManager.hpp"
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../utils.hpp"
#include "../../geometry/api/shapeBuilder.hpp"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GlutManager* GlutManager::instance = nullptr;

GlutManager::GlutManager() {
	this->instance = this;
	this->shapes = shapes;
	this->projectionMatrixUniform = 0;
	this->viewMatrixUniform = 0;
	this->modelMatrixUniform = 0;
	this->viewPositionUniform = 0;
	this->projectionMatrix = mat4(0.0f);
	this->viewMatrix = mat4(0.0f);
	this->camera = new Camera();
	this->shadersManager = new ShadersManager(
		(char*)"\\src\\shaderFiles\\vertexShader.glsl",
		(char*)"\\src\\shaderFiles\\fragmentShader.glsl"
	);
}

void GlutManager::openWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(APP_WIDTH, APP_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(APP_NAME);

	glutDisplayFunc(drawSceneAccessor);
	glutKeyboardFunc(movementAccessor);
	glutSpecialFunc(specialKeyAccessor);
	glutMouseWheelFunc(zoomAccessor);
	glutPassiveMotionFunc(lookAroundAccessor);
	glutTimerFunc(17, update, 0);

	glewExperimental = GL_TRUE;
	glewInit();
	this->shadersManager->createProgram();
	// init scene

	auto square1 = ShapeBuilder::createBox(1.0f, 1.0f, 1.0f, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	auto square2 = ShapeBuilder::createBox(1.0f, 1.0f, 1.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Shape* box1 = new Shape(square1.first, square1.second, 0.0f, 0.0f);
	Shape* box2 = new Shape(square2.first, square2.second, 0.0f, 0.0f);
	box2->move(0.5f, 0.5f, -0.5f);
	this->shapes.push_back(box1);
	this->shapes.push_back(box2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->projectionMatrixUniform = glGetUniformLocation(this->shadersManager->getProgramId(), "projection");
	this->modelMatrixUniform = glGetUniformLocation(this->shadersManager->getProgramId(), "model");
	this->viewMatrixUniform = glGetUniformLocation(this->shadersManager->getProgramId(), "view");
	this->viewPositionUniform = glGetUniformLocation(this->shadersManager->getProgramId(), "viewPos");

	glutMainLoop();
}

void GlutManager::drawSceneAccessor() {
	instance->drawScene();
}

void GlutManager::drawScene(void) {
	this->projectionMatrix = perspective(
		radians(this->camera->getPerspective()->getFOV()),
		this->camera->getPerspective()->getAspectRatio(),
		this->camera->getPerspective()->getNearPlane(),
		this->camera->getPerspective()->getFarPlane()
	);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(this->shadersManager->getProgramId());
	glUniformMatrix4fv(this->projectionMatrixUniform, 1, GL_FALSE, value_ptr(this->projectionMatrix));

	// Creates the view matrix applied to the vertices (world coordinates), transforming them in camera coordinates.
	this->viewMatrix = lookAt(
		vec3(this->camera->getView()->getPosition()),
		vec3(this->camera->getView()->getTarget()),
		vec3(this->camera->getView()->getUpvector())
	);

	glUniformMatrix4fv(this->viewMatrixUniform, 1, GL_FALSE, value_ptr(this->viewMatrix));
	glPointSize(10.0f);

	for (Shape* shape : this->shapes) {
		glUniformMatrix4fv(this->modelMatrixUniform, 1, GL_FALSE, value_ptr(shape->getModel()));
		glBindVertexArray(*shape->getVAO());

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_LINES, (shape->getIndices().size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glutSwapBuffers();
}

void GlutManager::movementAccessor(unsigned char key, int x, int y) {
	vec4 direction = instance->camera->getView()->getDirection();
	vec4 upVector = instance->camera->getView()->getUpvector();

	vec3 horizontalMovement = cross(vec3(direction), vec3(upVector)) * CAMERA_SPEED;
	vec3 verticalMovement = upVector * CAMERA_SPEED;
	vec3 depthMovement = vec3(direction) * CAMERA_SPEED;

	switch (key)
	{
		case 'a': case 'A': // Left
			instance->moveCamera(-horizontalMovement.x, -horizontalMovement.y, -horizontalMovement.z);
			break;
		case 'd': case 'D': // Right
			instance->moveCamera(horizontalMovement.x, horizontalMovement.y, horizontalMovement.z);
			break;
		case 32: // Up
			instance->moveCamera(verticalMovement.x, verticalMovement.y, verticalMovement.z);
			break;
		case 'w': case 'W': // Forward
			instance->moveCamera(depthMovement.x, depthMovement.y, depthMovement.z);
			break;
		case 's': case 'S': // Backward
			instance->moveCamera(-depthMovement.x, -depthMovement.y, -depthMovement.z);
			break;
		case 27:
			glutLeaveMainLoop();
			break;
		default:
			int modifiers = glutGetModifiers();
			if (modifiers & GLUT_ACTIVE_SHIFT) {
				// Shift key is pressed
				instance->moveCamera(-verticalMovement.x, -verticalMovement.y, -verticalMovement.z);
			}
			break;
	}
}

void GlutManager::moveCamera(float x, float y, float z) {
	this->camera->getView()->move(vec4(x, y, z, 1.0f));
}

void GlutManager::zoomAccessor(int wheel, int direction, int x, int y) {
	instance->zoomCamera(-direction);
}

void GlutManager::zoomCamera(int direction) {
	this->camera->getPerspective()->zoom(CAMERA_ZOOM * direction);
}

void GlutManager::lookAroundAccessor(int x, int y) {
	instance->lookAround(x, y);
}

void GlutManager::lookAround(int x, int y) {
	static int mouseX = x;
	static int mouseY = y;
	static float theta = -90.0f;
	static float phi = 0.0f;

	float alfa = 0.05;
	float centerX = APP_WIDTH / 2.0f;
	float centerY = APP_HEIGHT / 2.0f;

	float xoffset = x - centerX;
	float yoffset = APP_HEIGHT - y - centerY;

	mouseX = x;
	mouseY = y;

	xoffset *= alfa;
	yoffset *= alfa;
	theta += xoffset;
	phi += yoffset;

	if (phi > 90.0f)
		phi = 90.0f;
	if (phi < -90.0f)
		phi = -90.0f;

	vec3 front = vec3(
		cos(radians(theta)) * cos(radians(phi)),
		sin(radians(phi)),
		sin(radians(theta)) * cos(radians(phi))
	);
	vec4 position = this->camera->getView()->getPosition();
	vec4 direction = vec4(normalize(front), 0.0f);
	this->camera->getView()->setTarget(vec3(position + direction));

	glutWarpPointer(centerX, centerY);
}

void GlutManager::specialKeyAccessor(int key, int x, int y) {
	vec4 upVector = instance->camera->getView()->getUpvector();
	vec3 verticalMovement = upVector * CAMERA_SPEED;

	if (key == GLUT_KEY_SHIFT_L)
		instance->moveCamera(-verticalMovement.x, -verticalMovement.y, -verticalMovement.z);
}

void GlutManager::update(int value) {
	glutTimerFunc(17, update, 0); // 60 fps
	glutPostRedisplay();
}