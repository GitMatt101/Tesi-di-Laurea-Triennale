#include "../api/knapsackSolver.hpp"
#include "../api/solverUtils.hpp"
#include <iostream>
#include <algorithm>
#include <map>

constexpr auto MAX_ITERATIONS = 1000;

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

vector<pair<Box*, vec3>> KnapsackSolver::solve3D(vector<Box*> boxes) const {
	vector<Box*> currentSolution = createInitialSolution(boxes, this->maxWeight, this->containerSize);
	if (currentSolution.size() == boxes.size())
		return createSolutionFromBoxes(currentSolution);
	vector<vector<Box*>> solutionsBoxes = { currentSolution };
	vector<vector<vec3>> solutionsPositions = { getBoxesCoordinates(currentSolution) };

	for (int i = MAX_ITERATIONS; i > 0; i--) {
		vector<Box*> boxesLeft = getDifference(boxes, currentSolution);
		vector<Box*> neighbor = createNeighborSolution(currentSolution, boxesLeft);
		if (getWeight(neighbor) > this->maxWeight || !fits(neighbor, this->containerSize))
			continue;

		int currentProfit = getProfit(currentSolution);
		int neighborProfit = getProfit(neighbor);
		bool accept = false;
		if (neighborProfit >= currentProfit) {
			accept = true;
		} else {
			float p = rand() % 1001 / 1000.0f;
			int delta = neighborProfit - currentProfit;
			if (exp(delta / i) > p)
				accept = true;
		}
		if (accept) {
			currentSolution = neighbor;
			auto it = find(solutionsBoxes.begin(), solutionsBoxes.end(), neighbor);
			if (it == solutionsBoxes.end()) {
				solutionsBoxes.push_back(neighbor);
				solutionsPositions.push_back(getBoxesCoordinates(neighbor));
			}
		}
	}
	vector<Box*> maxBoxes = {};
	vector<vec3> maxPositions = {};
	for (int i = 0; i < solutionsBoxes.size(); i++) {
		if (getProfit(solutionsBoxes[i]) > getProfit(maxBoxes)) {
			maxBoxes = solutionsBoxes[i];
			maxPositions = solutionsPositions[i];
		}
	}
	vector<pair<Box*, vec3>> finalSolution;
	for (int i = 0; i < maxBoxes.size(); i++)
		finalSolution.push_back({ maxBoxes[i], maxPositions[i] });
	return finalSolution;
}