#include "./glut/api/glutManager.hpp"
#include "./geometry/api/shapeBuilder.hpp"
#include "./solver/api/knapsackSolver.hpp"
#include "./solver/api/solverUtils.hpp"
#include <iostream>
#include <time.h>
#include <chrono>
#include <fstream>
#include <direct.h>
#include <algorithm>
using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));

	char cwd[1024];
	if (_getcwd(cwd, sizeof(cwd)) == nullptr) {
		fprintf(stderr, "ERROR - LOADING CURRENT DIRECTORY FAILED\n");
		exit(-1);
	}
	strcat_s(cwd, sizeof(cwd), "\\src\\main\\kp.txt");

	// Open file
	ifstream file(cwd);
	if (!file) {
		cerr << "Failed to load file" << endl;
		exit(-1);
	}

	vec3 containerSize = vec3(0);
	int maxWeight;
	vector<Shape*> boxes;

	#pragma region Read File
	file >> containerSize.x >> containerSize.y >> containerSize.z;
	file >> maxWeight;

	int width, height, depth, profit, weight;
	float r, g, b;
	int i = 1;
	while (file >> width >> height >> depth >> profit >> weight >> r >> g >> b) {
		auto bVal = ShapeBuilder::createBox(width, height, depth, vec4(r, g, b, 1));
		Shape* shape = new Shape(bVal.first, bVal.second, i, profit, weight);
		i++;
		boxes.push_back(shape);
	}
	#pragma endregion

	KnapsackSolver* ks = new KnapsackSolver(containerSize, maxWeight);
	auto start = std::chrono::high_resolution_clock::now();
	vector<Shape*> solution = ks->solve3D(boxes);
	auto end = std::chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;

	#pragma region Print Solution
	cout << "Boxes: ";
	int totalProfit = 0, totalWeight = 0;
	for (Shape* box : solution) {
		cout << box->getId() << " ";
		totalProfit += box->getValue();
		totalWeight += box->getWeight();
	}
	cout << "\nProfit: " << totalProfit << "\tWeight: " << totalWeight << "/" << maxWeight << endl;
	cout << "Elapsed Time: " << elapsed.count() << " seconds" << endl;
	#pragma endregion

	#pragma region Prepare Scene
	vector<Shape*> scene;
	auto containerValues = ShapeBuilder::createBox(containerSize.x, containerSize.y, containerSize.z, vec4(1.0f));
	Shape* container = new Shape(containerValues.first, containerValues.second, 0, 0.0f, 0.0f);
	scene.push_back(container);

	vector<Shape*> placedBoxes;
	for (Shape* box : solution) {
		vec3 c = getCoordinates(placedBoxes, box, containerSize);

		vec3 boxSize = box->getSize();
		sort(placedBoxes.begin(), placedBoxes.end(), [](Shape* a, Shape* b) { return a->getPosition().y < b->getPosition().y; });
		vector<Shape*> boxesToReplace;
		if (box == solution[solution.size() - 1]) {
			for (Shape* placedBox : placedBoxes) {
				vec3 pPos = placedBox->getPosition();
				vec3 pSize = placedBox->getSize();
				if (c.z < pPos.z
					&& c.x + boxSize.x > pPos.x && c.x < pPos.x + pSize.x
					&& c.y + boxSize.y > pPos.y && c.y < pPos.y + pSize.y) {
					boxesToReplace.push_back(placedBox);
					placedBoxes.erase(remove(placedBoxes.begin(), placedBoxes.end(), placedBox), placedBoxes.end());
					scene.erase(remove(scene.begin(), scene.end(), placedBox), scene.end());
				}
			}
		}

		box->setPosition(c);
		box->setTarget(c);
		placedBoxes.push_back(box);
		scene.push_back(box);

		if (box == solution[solution.size() - 1]) {
			for (Shape* boxToPlace : boxesToReplace) {
				placedBoxes.push_back(boxToPlace);
				scene.push_back(boxToPlace);
			}
		}
	}
	for (Shape* box : placedBoxes)
		box->setStartPosition(vec3(box->getPosition().x, box->getPosition().y, rand() % 20 + 10));
	#pragma endregion

	GlutManager* glutManager = new GlutManager(scene);
	glutManager->openWindow(argc, argv);
}