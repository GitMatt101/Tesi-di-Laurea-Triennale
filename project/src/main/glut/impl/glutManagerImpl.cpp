#include "../api/glutManager.hpp"
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../utils.hpp"
#include "../../geometry/api/shapeBuilder.hpp"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

vector<GLuint> linesIndices = {
	// Front face
	0, 1, 1, 2, 2, 3, 3, 0,
	// Back face
	4, 5, 5, 6, 6, 7, 7, 4,
	// Connecting edges
	0, 4, 1, 5, 2, 6, 3, 7
};
vector<GLuint> trianglesIndices = {
	// Front Face
	0, 1, 2, 2, 3, 0,
	// Right Face
	1, 5, 6, 6, 2, 1,
	// Back Face
	7, 6, 5, 5, 4, 7,
	// Left Face
	4, 0, 3, 3, 7, 4,
	// Top Face
	4, 5, 1, 1, 0, 4,
	// Bottom Face
	3, 2, 6, 6, 7, 3
};

GlutManager* GlutManager::instance = nullptr;

GlutManager::GlutManager(vector<Box*> boxes) {
	this->instance = this;
	this->boxes = boxes;
	this->projectionMatrixUniform = 0;
	this->viewMatrixUniform = 0;
	this->modelMatrixUniform = 0;
	this->viewPositionUniform = 0;
	this->projectionMatrix = mat4(0.0f);
	this->viewMatrix = mat4(0.0f);
	this->polygonMode = GL_FILL;
	this->elementsMode = GL_TRIANGLES;
	this->camera = new Camera();
	this->shadersManager = new ShadersManager(
		(char*)"\\src\\shaderFiles\\vertexShader.glsl",
		(char*)"\\src\\shaderFiles\\fragmentShader.glsl"
	);
	this->boxes[0]->setIndices(linesIndices);
	this->pause = false;
	this->insert = true;
}

void GlutManager::openWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(APP_WIDTH, APP_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(APP_NAME);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(moveCamera);
	glutMouseWheelFunc(zoomCamera);
	glutPassiveMotionFunc(lookAround);
	glutTimerFunc(FRAME_LENGTH, update, 0);

	glewExperimental = GL_TRUE;
	glewInit();
	this->shadersManager->createProgram();

	for (Box* shape : this->boxes)
		shape->init();

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

void GlutManager::drawScene(void) {
	instance->projectionMatrix = perspective(
		radians(instance->camera->getPerspective()->getFOV()),
		instance->camera->getPerspective()->getAspectRatio(),
		instance->camera->getPerspective()->getNearPlane(),
		instance->camera->getPerspective()->getFarPlane()
	);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Background Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(instance->shadersManager->getProgramId());
	glUniformMatrix4fv(instance->projectionMatrixUniform, 1, GL_FALSE, value_ptr(instance->projectionMatrix));

	instance->viewMatrix = lookAt(
		vec3(instance->camera->getView()->getPosition()),
		vec3(instance->camera->getView()->getTarget()),
		vec3(instance->camera->getView()->getUpvector())
	);

	glUniformMatrix4fv(instance->viewMatrixUniform, 1, GL_FALSE, value_ptr(instance->viewMatrix));
	glPointSize(10.0f);

	Box* container = instance->boxes[0];
	glUniformMatrix4fv(instance->modelMatrixUniform, 1, GL_FALSE, value_ptr(container->getModel()));
	glBindVertexArray(*container->getVAO());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_LINES, (container->getIndices().size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (int i = 1; i < instance->boxes.size(); i++) {
		Box* box = instance->boxes[i];
		glUniformMatrix4fv(instance->modelMatrixUniform, 1, GL_FALSE, value_ptr(box->getModel()));
		glBindVertexArray(*box->getVAO());
		glPolygonMode(GL_FRONT_AND_BACK, instance->polygonMode);
		glDrawElements(instance->elementsMode, (box->getIndices().size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	glutSwapBuffers();
}

void GlutManager::moveCamera(unsigned char key, int x, int y) {
	vec4 direction = instance->camera->getView()->getDirection();
	vec4 upVector = instance->camera->getView()->getUpvector();

	vec3 horizontalMovement = cross(vec3(direction), vec3(upVector)) * CAMERA_SPEED;
	vec3 verticalMovement = upVector * CAMERA_SPEED;
	vec3 depthMovement = vec3(direction) * CAMERA_SPEED;

	switch (key)
	{
		case 'a': case 'A': // Left
			instance->camera->getView()->move(vec4(-horizontalMovement, 1.0f));
			break;
		case 'd': case 'D': // Right
			instance->camera->getView()->move(vec4(horizontalMovement, 1.0f));
			break;
		case 'e': case 'E': // Up
			instance->camera->getView()->move(vec4(verticalMovement, 1.0f));
			break;
		case 'q': case 'Q': // Down
			instance->camera->getView()->move(vec4(-verticalMovement, 1.0f));
			break;
		case 'w': case 'W': // Forward
			instance->camera->getView()->move(vec4(depthMovement, 1.0f));
			break;
		case 's': case 'S': // Backward
			instance->camera->getView()->move(vec4(-depthMovement, 1.0f));
			break;
		case 'k': case 'K':
			instance->camera->getView()->setTarget(vec3(6.0f, 3.0f, 3.0f));
			instance->camera->getView()->setPosition(vec3(-2.0f, 12.0f, 22.0f));
			break;
		case 'l': case 'L':
			instance->camera->getView()->setTarget(vec3(3.0f, 3.0f, 3.0f));
			instance->camera->getView()->setPosition(vec3(11.0f, 12.0f, 22.0f));
			break;
		case 'p': case 'P':
			instance->pause = !instance->pause;
			break;
		case 'c': case 'C':
			instance->pause = false;
			instance->insert = true;
			break;
		case 'b': case 'B':
			instance->pause = false;
			instance->insert = false;
			break;
		case 'm': case 'M':
			if (instance->polygonMode == GL_FILL) {
				instance->polygonMode = GL_LINE;
				instance->elementsMode = GL_LINES;
				for (int i = 1; i < instance->boxes.size(); i++) {
					Box* shape = instance->boxes[i];
					shape->setIndices(linesIndices);
					shape->init();
				}
			} else {
				instance->polygonMode = GL_FILL;
				instance->elementsMode = GL_TRIANGLES;
				for (int i = 1; i < instance->boxes.size(); i++) {
					Box* shape = instance->boxes[i];
					shape->setIndices(trianglesIndices);
					shape->init();
				}
			}
			break;
		case 27:
			glutLeaveMainLoop();
			break;
		default:
			break;
	}
}

void GlutManager::zoomCamera(int wheel, int direction, int x, int y) {
	instance->camera->getPerspective()->zoom(CAMERA_ZOOM * -direction);
}

void GlutManager::lookAround(int x, int y) {
	// Initialization
	static int mouseX = x;	
	static int mouseY = y;
	static float theta = -90.0f;
	static float phi = 0.0f;

	float centerX = APP_WIDTH / 2.0f;
	float centerY = APP_HEIGHT / 2.0f;
	mouseX = x;
	mouseY = y;

	float xoffset = (x - centerX) * 0.05f;
	float yoffset = (APP_HEIGHT - y - centerY) * 0.05f;
	theta += xoffset;
	phi += yoffset;

	if (phi > 90.0f) phi = 90.0f;
	if (phi < -90.0f) phi = -90.0f;
	vec3 front = vec3(
		cos(radians(theta)) * cos(radians(phi)),
		sin(radians(phi)),
		sin(radians(theta)) * cos(radians(phi))
	);
	vec4 position = instance->camera->getView()->getPosition();
	vec4 direction = vec4(normalize(front), 0.0f);
	instance->camera->getView()->setTarget(vec3(position + direction));

	// Keep cursor on the center of the window
	glutWarpPointer(centerX, centerY);
}

void GlutManager::update(int value) {
	if (!instance->pause) {
		if (instance->insert) {
			for (int i = 0; i < instance->boxes.size(); i++) {
				if (!instance->boxes[i]->targetReached()) {
					instance->boxes[i]->moveTowardsTarget();
					break;
				}
			}
		} else {
			for (int i = instance->boxes.size() - 1; i >= 0; i--) {
				if (!instance->boxes[i]->startReached()) {
					instance->boxes[i]->moveTowardsStart();
					break;
				}
			}
		}
	}
	glutTimerFunc(FRAME_LENGTH, update, 0);
	glutPostRedisplay();
}