#include "../api/knapsackSolver.hpp"

/// <summary>
/// Given a 3D space, this method calculates the biggest 3D rectangle that can fit in said space.
/// </summary>
/// <param name="width">- Width of the space.</param>
/// <param name="height">- Height of the space.</param>
/// <param name="depth">- Depth of the space.</param>
/// <param name="placedBoxes">- Boxed (supposedly not overlapping) already placed in the space.</param>
/// <returns>The biggest box that can be placed in the space as a tuple of width, height and depth.</returns>
static tuple<float, float, float> availableSpace(float width, float height, float depth, vector<Shape*> placedBoxes) {

}

KnapsackSolver::KnapsackSolver(float width, float height, float depth, float maxWeight) {
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->maxWeight = maxWeight;
	this->emptySpace = tuple<float, float, float>(width, height, depth);
}

vector<Shape*> KnapsackSolver::solve3D(vector<Shape*> boxes) const {
    
	return boxes;
}