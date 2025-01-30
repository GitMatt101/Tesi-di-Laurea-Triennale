#include "./glut/api/glutManager.hpp"
#include "./geometry/api/shapeBuilder.hpp"
#include "./solver/api/knapsackSolver.hpp"
#include <iostream>
#include <tuple>
#include <time.h>
#include <algorithm>
#include <map>
using namespace std;

struct Box {
	int width, height, depth;
	int value, weight;
};

int main(int argc, char **argv) {
	srand(time(NULL));
	vector<Shape*> scene;

	auto containerValues = ShapeBuilder::createBox(9.0f, 9.0f, 9.0f, vec4(1.0f));
	Shape* container = new Shape(containerValues.first, containerValues.second, 0, 0.0f, 0.0f);
	scene.push_back(container);

	vector<Box> boxesValues = {
		{ 2, 4, 9, 3, 5 },
		{ 3, 7, 3, 5, 3 },
		{ 3, 7, 6, 4, 1 },
		{ 5, 2, 3, 7, 8 },
		{ 5, 2, 6, 9, 9 },
		{ 6, 3, 3, 3, 4 },
		{ 6, 3, 6, 5, 4 },
		{ 4, 6, 3, 6, 7 },
		{ 4, 6, 6, 6, 4 }
	};
	vector<Shape*> boxes;
	for (int i = 0; i < boxesValues.size(); i++) {
		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		auto bVal = ShapeBuilder::createBox(boxesValues[i].width, boxesValues[i].height, boxesValues[i].depth, vec4(r, g, b, 1.0f));
		Shape* shape = new Shape(
			bVal.first,
			bVal.second,
			i + 1,
			boxesValues[i].value, boxesValues[i].weight
		);
		boxes.push_back(shape);
	}

	KnapsackSolver* ks = new KnapsackSolver(9.0f, 9.0f, 9.0f, 100);
	vector<Shape*> solution = ks->solve3D(boxes);
	int profit = 0;
	int weight = 0;
	vector<Shape*> placedBoxes;
	std::cout << "Chosen Boxes: ";
	for (Shape* box : solution) {
		std::cout << box->getId() << " ";
		profit += box->getValue();
		weight += box->getWeight();
		vec3 c = getCoordinates(placedBoxes, box, vec3(9.0f, 9.0f, 9.0f));

		vec3 boxSize = box->getSize();
		vector<Shape*> boxesToReplace;
		for (Shape* placedBox : placedBoxes) {
			vec3 pPos = placedBox->getPosition();
			vec3 pSize = placedBox->getSize();
			if (c.z < pPos.z
				&& c.x + boxSize.x > pPos.x && c.x < pPos.x + pSize.x
				&& c.y + boxSize.y > pPos.y && c.y < pPos.y + pSize.y) {
				placedBoxes.erase(remove(placedBoxes.begin(), placedBoxes.end(), placedBox), placedBoxes.end());
				scene.erase(remove(scene.begin(), scene.end(), placedBox), scene.end());
			}
		}

		box->setPosition(c);
		box->setTarget(c);
		placedBoxes.push_back(box);
		scene.push_back(box);

		for (Shape* boxToPlace : boxesToReplace) {
			placedBoxes.push_back(box);
			scene.push_back(box);
		}
	}
	for (Shape* box : placedBoxes)
		box->setStartPosition(vec3(box->getPosition().x, box->getPosition().y, rand() % 20 + 10));
	std::cout << "\nProfit: " << profit << endl;
	std::cout << "Weight: " << weight << "/" << 30 << endl;

	GlutManager* glutManager = new GlutManager(scene);
	glutManager->openWindow(argc, argv);
}