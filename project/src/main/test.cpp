#include "./glut/api/glutManager.hpp"
#include "./geometry/api/shapeBuilder.hpp"

int main(int argc, char **argv) {
	GlutManager* glutManager = new GlutManager();
	glutManager->openWindow(argc, argv);
}