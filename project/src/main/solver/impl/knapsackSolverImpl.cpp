#include "../api/knapsackSolver.hpp"
#include "../api/solverUtils.hpp"
#include <iostream>
#include <algorithm>
#include <map>

constexpr auto MAX_ITERATIONS = 10000;

KnapsackSolver::KnapsackSolver(vec3 containerSize, float maxWeight) {
	this->containerSize = containerSize;
	this->maxWeight = maxWeight;
}

vector<Box*> KnapsackSolver::solve3D(vector<Box*> boxes) const {
	vector<Box*> initialSolution;
	do {
		initialSolution.clear();
		for (Box* box : boxes) {
			if (rand() % 2 == 1)
				initialSolution.push_back(box);
		}
	} while (getWeight(initialSolution) > this->maxWeight || !fits(initialSolution, this->containerSize));

	vector<vector<Box*>> solutions;
	solutions.push_back(initialSolution);
	for (int i = MAX_ITERATIONS; i > 0; i--) {
		vector<Box*> neighbor = initialSolution;
		vector<Box*> boxesLeft = getDifference(boxes, neighbor);
		if (boxesLeft.size() == 0) {
			if (getWeight(neighbor) <= this->maxWeight && fits(neighbor, this->containerSize))
				return neighbor;
			else
				continue;
		}
		if (neighbor.size() == 0) {
			neighbor.push_back(boxesLeft[rand() % boxesLeft.size()]);
		} else {
			int r = rand() % 3;
			switch (r) {
				case 0:
					neighbor.erase(neighbor.begin() + rand() % neighbor.size());
					break;
				case 1:
					neighbor.push_back(boxesLeft[rand() % boxesLeft.size()]);
					break;
				case 2:
					neighbor[rand() % neighbor.size()] = boxesLeft[rand() % boxesLeft.size()];
					break;
			}
		}
		if (getWeight(neighbor) > this->maxWeight)
			continue;
		int initialValue = getProfit(initialSolution);
		int neighborValue = getProfit(neighbor);
		bool accept = false;
		auto it = find(solutions.begin(), solutions.end(), neighbor);
		if (neighborValue >= initialValue) {
			if (it != solutions.end())
				accept = true;
			else if (fits(neighbor, this->containerSize))
				accept = true;
		} else {
			float p = rand() % 1001 / 1000.0f;
			int delta = neighborValue - initialValue;
			if (it != solutions.end() && exp(delta / i) > p)
				accept = true;
			else if (exp(delta / i) > p && fits(neighbor, this->containerSize))
				accept = true;
		}
		if (accept) {
			initialSolution = neighbor;
			if (it == solutions.end())
				solutions.push_back(neighbor);
		}
	}
	vector<Box*> max;
	for (vector<Box*> solution : solutions) {
		if (getProfit(solution) > getProfit(max))
			max = solution;
	}
	return max;
}