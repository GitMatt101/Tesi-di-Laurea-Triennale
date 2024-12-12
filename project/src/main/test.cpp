#include "shaders/api/shadersManager.hpp"
#include "geometry/api/shapeBuilder.hpp"
#include "geometry/api/shape.hpp"

int main(int argc, char **argv) {
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboardEvent);
	glutMouseWheelFunc(zoom);
	glutPassiveMotionFunc(passiveCameraMovement);
	glutTimerFunc(100, update, 0);

	glewExperimental = GL_TRUE;

	glewInit();
	initShader();
	initMaterials();
	initShaders();
	initCubeMap();
	initCamera();
	initScene();
	initTextVAO(&textVAO, &textVBO);
	initFreetype();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Parametri per le mesh
	uniformProjectionMatrix = glGetUniformLocation(programId, "Projection");
	uniformModelMatrix = glGetUniformLocation(programId, "Model");
	uniformViewMatrix = glGetUniformLocation(programId, "View");
	uniformShader = glGetUniformLocation(programId, "shader");
	uniformTime = glGetUniformLocation(programId, "time");
	uniformViewPosition = glGetUniformLocation(programId, "viewPos");
	uniformTexture = glGetUniformLocation(programId, "tex");
	uniformLight.light_position_pointer = glGetUniformLocation(programId, "light.position");
	uniformLight.light_color_pointer = glGetUniformLocation(programId, "light.color");
	uniformLight.light_power_pointer = glGetUniformLocation(programId, "light.power");
	uniformLight.material_ambient = glGetUniformLocation(programId, "material.ambient");
	uniformLight.material_diffuse = glGetUniformLocation(programId, "material.diffuse");
	uniformLight.material_specular = glGetUniformLocation(programId, "material.specular");
	uniformLight.material_shininess = glGetUniformLocation(programId, "material.shininess");

	// Parametri per la skybox
	uniformProjectionMatrixCubeMap = glGetUniformLocation(programId_cubemap, "Projection");
	uniformViewMatrixCubeMap = glGetUniformLocation(programId_cubemap, "View");

	// Parametri per la riflessione
	uniformModelMatrixReflection = glGetUniformLocation(programId_reflection, "Model");
	uniformViewMatrixReflection = glGetUniformLocation(programId_reflection, "View");
	uniformProjectionMatrixReflection = glGetUniformLocation(programId_reflection, "Projection");
	uniformViewPositionReflection = glGetUniformLocation(programId_reflection, "viewPos");
	uniformCubeMapReflection = glGetUniformLocation(programId_reflection, "cubemap");

	glutMainLoop();
}