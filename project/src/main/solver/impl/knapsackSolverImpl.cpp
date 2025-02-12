#include "../api/knapsackSolver.hpp"
#include "../api/solverUtils.hpp"
#include <iostream>
#include <algorithm>
#include <map>

constexpr auto MAX_ITERATIONS = 10000;

static vector<Box*> createNeighborSolution(vector<Box*> currentSolution, vector<Box*> remainingBoxes) {
	if (currentSolution.size() == 0)
		return { remainingBoxes[rand() % remainingBoxes.size()] };

	vector<Box*> neighbor = currentSolution;
	switch (rand() % 3) {
		case 0:
			neighbor.erase(neighbor.begin() + rand() % neighbor.size());
			break;
		case 1:
			neighbor.push_back(remainingBoxes[rand() % remainingBoxes.size()]);
			break;
		case 2:
			neighbor[rand() % neighbor.size()] = remainingBoxes[rand() % remainingBoxes.size()];
			break;
	}
	return neighbor;
}

KnapsackSolver::KnapsackSolver(vec3 containerSize, float maxWeight) {
	this->containerSize = containerSize;
	this->maxWeight = maxWeight;
}

vector<Box*> KnapsackSolver::solve3D(vector<Box*> boxes) const {
	vector<Box*> initialSolution;
	for (Box* box : boxes) {
		if (rand() % 2 == 1)
			initialSolution.push_back(box);
	}
	vector<vector<Box*>> solutions;
	for (int i = MAX_ITERATIONS; i > 0; i--) {
		vector<Box*> boxesLeft = getDifference(boxes, initialSolution);
		if (boxesLeft.size() == 0) {
			if (getWeight(initialSolution) <= this->maxWeight && fits(initialSolution, this->containerSize))
				return initialSolution;
			else
				continue;
		}
		vector<Box*> neighbor = createNeighborSolution(initialSolution, boxesLeft);
		if (getWeight(neighbor) > this->maxWeight)
			continue;

		int initialValue = getProfit(initialSolution);
		int neighborValue = getProfit(neighbor);
		bool accept = false;
		auto it = find(solutions.begin(), solutions.end(), neighbor);
		if (neighborValue >= initialValue) {
			if (it != solutions.end() || fits(neighbor, this->containerSize))
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