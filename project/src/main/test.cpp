#include "./glut/api/glutManager.hpp"
#include "./geometry/api/shapeBuilder.hpp"
#include "./solver/api/knapsackSolver.hpp"
#include "./solver/api/solverUtils.hpp"
#include <iostream>
#include <tuple>
#include <time.h>
#include <algorithm>
#include <chrono>
using namespace std;

struct Box {
	int width, height, depth;
	int value, weight;
	vec4 color;
};

bool static compareVectors(vector<Shape*> s1, vector<Shape*> s2) {
	if (s1.size() != s1.size())
		return false;
	for (int i = 0; i < s1.size(); i++) {
		auto it = find(s2.begin(), s2.end(), s1[i]);
		if (it != s2.end())
			continue;
		else
			return false;
	}
	return true;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	vector<Shape*> scene;

	vec3 containerSize = vec3(9);
	int maxWeight = 30;
	auto containerValues = ShapeBuilder::createBox(containerSize.x, containerSize.y, containerSize.z, vec4(1.0f));
	Shape* container = new Shape(containerValues.first, containerValues.second, 0, 0.0f, 0.0f);
	scene.push_back(container);

	vector<Box> boxesValues = {
		{ 2, 4, 9, 3, 5, vec4(1, 0, 0, 1) },
		{ 3, 7, 3, 5, 3, vec4(0, 1, 0, 1) },
		{ 3, 7, 6, 4, 1, vec4(0, 0, 1, 1) },
		{ 5, 2, 3, 7, 8, vec4(1, 1, 0, 1) },
		{ 5, 2, 6, 9, 9, vec4(1, 0, 1, 1) },
		{ 6, 3, 3, 3, 4, vec4(0, 1, 1, 1) },
		{ 6, 3, 6, 5, 4, vec4(0.7, 0.2, 0.2, 1) },
		{ 4, 6, 3, 6, 7, vec4(0.2, 0.7, 0.2, 1) },
		{ 4, 6, 6, 6, 4, vec4(0.2, 0.2, 0.7, 1) }
		/*{ 2, 2, 3, 4, 6, vec4(0.7, 0.7, 0.2, 1) },
		{ 6, 2, 7, 7, 5, vec4(0.7, 0.2, 0.7, 1) },
		{ 1, 8, 2, 4, 6, vec4(0.2, 0.7, 0.7, 1) }*/
	};
	vector<Shape*> boxes;
	for (int i = 0; i < boxesValues.size(); i++) {
		auto bVal = ShapeBuilder::createBox(boxesValues[i].width, boxesValues[i].height, boxesValues[i].depth, boxesValues[i].color);
		Shape* shape = new Shape(
			bVal.first,
			bVal.second,
			i + 1,
			boxesValues[i].value, boxesValues[i].weight
		);
		boxes.push_back(shape);
	}

	KnapsackSolver* ks = new KnapsackSolver(containerSize, maxWeight);
	vector<pair<vector<Shape*>, int>> solutions;
	vector<int> profits;
	vector<int> weights;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1; i++) {
		vector<Shape*> solution = ks->solve3D(boxes);
		int profit = 0;
		int weight = 0;
		for (Shape* box : solution) {
			profit += box->getValue();
			weight += box->getWeight();
		}
		if (solutions.size() == 0) {
			solutions.push_back({ solution, 1 });
			continue;
		}
		bool found = false;
		for (int i = 0; i < solutions.size(); i++) {
			if (compareVectors(solutions[i].first, solution)) {
				solutions[i].second++;
				found = true;
				break;
			}
		}
		if (!found) {
			solutions.push_back({ solution, 1 });
		}
		cout << endl << "--------------------" << i << "--------------------" << endl;
	}
	auto end = std::chrono::high_resolution_clock::now();
	sort(solutions.begin(), solutions.end(), [](pair<vector<Shape*>, int> a, pair<vector<Shape*>, int> b) { return getProfit(a.first) > getProfit(b.first); });
	for (int i = 0; i < solutions.size(); i++) {
		std::cout << "Boxes: ";
		int profit = 0, weight = 0;
		for (int j = 0; j < solutions[i].first.size(); j++) {
			std::cout << solutions[i].first[j]->getId() << " ";
			profit += solutions[i].first[j]->getValue();
			weight += solutions[i].first[j]->getWeight();
		}
		cout << endl << "Instances: " << solutions[i].second << "\tProfit: " << profit << "\tWeight: " << weight << "/" << maxWeight << endl << endl;
	}

	chrono::duration<double> elapsed = end - start;
	cout << "Elapsed Time: " << elapsed.count() << " seconds" << endl;

	vector<Shape*> placedBoxes;
	for (Shape* box : solutions[0].first) {
		vec3 c = getCoordinates(placedBoxes, box, containerSize);

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

	GlutManager* glutManager = new GlutManager(scene);
	glutManager->openWindow(argc, argv);
}