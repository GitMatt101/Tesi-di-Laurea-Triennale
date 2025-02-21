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
		fprintf(stderr, "ERROR LOADING CURRENT DIRECTORY\n");
		exit(-1);
	}
	strcat_s(cwd, sizeof(cwd), "\\src\\main\\kp.txt");

	// Open file
	ifstream file(cwd);
	if (!file) {
		std::cerr << "ERROR LOADING FILE" << endl;
		exit(-1);
	}

	vec3 containerSize = vec3(0);
	int maxWeight;
	file >> containerSize.x >> containerSize.y >> containerSize.z;
	file >> maxWeight;

	vector<Box*> boxes;
	int width, height, depth, profit, weight;
	float r, g, b;
	int i = 1;
	while (file >> width >> height >> depth >> profit >> weight >> r >> g >> b) {
		auto bVal = ShapeBuilder::createBox(width, height, depth, vec4(r, g, b, 1));
		Box* shape = new Box(bVal.first, bVal.second, i, profit, weight);
		i++;
		boxes.push_back(shape);
	}

	KnapsackSolver* ks = new KnapsackSolver(containerSize, maxWeight);

	auto start = std::chrono::high_resolution_clock::now();
	vector<pair<Box*, vec3>> solution = ks->solve3D(boxes);
	auto end = std::chrono::high_resolution_clock::now();

	chrono::duration<double> elapsed = end - start;
	cout << "Elapsed Time: " << elapsed.count() << " seconds" << endl;

	vector<Box*> scene;
	auto containerValues = ShapeBuilder::createBox(containerSize.x, containerSize.y, containerSize.z, vec4(1.0f));
	Box* container = new Box(containerValues.first, containerValues.second, 0, 0.0f, 0.0f);
	scene.push_back(container);

	cout << "Boxes: ";
	int totalProfit = 0, totalWeight = 0;
	for (pair<Box*, vec3> box : solution) {
		cout << box.first->getId() << " ";
		totalProfit += box.first->getValue();
		totalWeight += box.first->getWeight();

		box.first->setPosition(box.second);
		box.first->setTarget(box.second);
		box.first->setStartPosition(vec3(box.second.x, box.second.y, rand() % 20 + 20));
		scene.push_back(box.first);
	}
	cout << "\nProfit: " << totalProfit << "\tWeight: " << totalWeight << "/" << maxWeight << endl;

	GlutManager* glutManager = new GlutManager(scene);
	glutManager->openWindow(argc, argv);
}