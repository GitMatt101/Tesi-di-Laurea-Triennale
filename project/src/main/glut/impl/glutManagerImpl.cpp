#include "../api/glutManager.hpp"
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../utils.hpp"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GlutManager::GlutManager(unsigned int programId, vector<Shape*> shapes) {
	this->instance = this;
	this->programId = programId;
	this->shapes = shapes;
	this->projectionMatrixUniform = 0;
	this->viewMatrixUniform = 0;
	this->modelMatrixUniform = 0;
	this->projectionMatrix = mat4(0.0f);
	this->viewMatrix = mat4(0.0f);
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
}

void GlutManager::drawSceneAccessor() {
	instance->drawScene();
}

void GlutManager::drawScene(void) {
	this->projectionMatrix = perspective(radians(cameraPerspective.fov), cameraPerspective.aspect, cameraPerspective.nearPlane, cameraPerspective.farPlane);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(this->projectionMatrixUniform, 1, GL_FALSE, value_ptr(this->projectionMatrix));

	// Creates the view matrix applied to the vertices (world coordinates), transforming them in camera coordinates.
	this->viewMatrix = lookAt(vec3(camera.position), vec3(camera.target), vec3(camera.upVector));

	glUniformMatrix4fv(this->viewMatrixUniform, 1, GL_FALSE, value_ptr(this->viewMatrix));
	glPointSize(10.0f);
	for (Shape* shape : this->shapes) {
		shape->setAnchorWorld(shape->getAnchorObj());
		shape->setAnchorWorld(shape->getModel() * shape->getAnchorWorld());

		glUniformMatrix4fv(this->modelMatrixUniform, 1, GL_FALSE, value_ptr(shape->getModel()));
		glBindVertexArray(*shape->getVAO());

		unsigned int ind = shape->getIndices().size() - 1;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(ind * sizeof(GLuint)));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, shape->getIndices().size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	glutSwapBuffers();
	glUseProgram(this->programId);
}