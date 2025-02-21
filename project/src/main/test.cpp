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

struct BoxData {
	int width, height, depth;
	int value, weight;
	vec4 color;
};

static bool compareVectors(vector<pair<Box*, vec3>> s1, vector<pair<Box*, vec3>> s2) {
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

static int getProfitFromSolution(vector<pair<Box*, vec3>> solution) {
	int profit = 0;
	for (pair<Box*, vec3> box : solution)
		profit += box.first->getValue();
	return profit;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	vector<Box*> scene;

	vec3 containerSize = vec3(12);
	int maxWeight = 30;
	auto containerValues = ShapeBuilder::createBox(containerSize.x, containerSize.y, containerSize.z, vec4(1.0f));
	Box* container = new Box(containerValues.first, containerValues.second, 0, 0.0f, 0.0f);
	scene.push_back(container);

	vector<BoxData> boxesValues = {
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
	vector<Box*> boxes;
	for (int i = 0; i < boxesValues.size(); i++) {
		auto bVal = ShapeBuilder::createBox(boxesValues[i].width, boxesValues[i].height, boxesValues[i].depth, boxesValues[i].color);
		Box* box = new Box(
			bVal.first,
			bVal.second,
			i + 1,
			boxesValues[i].value, boxesValues[i].weight
		);
		boxes.push_back(box);
	}

	KnapsackSolver* ks = new KnapsackSolver(containerSize, maxWeight);
	vector<pair<vector<pair<Box*, vec3>>, int>> solutions;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		vector<pair<Box*, vec3>> solution = ks->solve3D(boxes);
		int profit = 0;
		int weight = 0;

		for (pair<Box*, vec3> box : solution) {
			profit += box.first->getValue();
			weight += box.first->getWeight();
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
	chrono::duration<double> elapsed = end - start;
	cout << "Elapsed Time: " << elapsed.count() << " seconds" << endl;

	sort(solutions.begin(), solutions.end(), [](pair<vector<pair<Box*, vec3>>, int> a, pair<vector<pair<Box*, vec3>>, int> b) { return getProfitFromSolution(a.first) > getProfitFromSolution(b.first); });
	for (int i = 0; i < solutions.size(); i++) {
		std::cout << "Boxes: ";
		int profit = 0, weight = 0;
		for (int j = 0; j < solutions[i].first.size(); j++) {
			std::cout << solutions[i].first[j].first->getId() << " ";
			profit += solutions[i].first[j].first->getValue();
			weight += solutions[i].first[j].first->getWeight();
		}
		cout << endl << "Instances: " << solutions[i].second << "\tProfit: " << profit << "\tWeight: " << weight << "/" << maxWeight << endl << endl;
	}
}