#pragma once
#include <vector>
#include "../../geometry/api/shape.hpp"
using namespace std;

/// <summary>
/// Class that solves the knapsack problem using dynamic programming.
/// </summary>
class KnapsackSolver {
	private:
		float width;
		float height;
		float depth;
		float maxWeight;
		tuple<float, float, float> emptySpace;

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		/// <param name="values"></param>
		KnapsackSolver(float width, float height, float depth, float maxWeight);

		/// <summary>
		/// Solves 3-dimensional Knapsack Problem.
		/// </summary>
		/// <param name="boxes"></param>
		/// <returns>A vector of boxes chosen among the ones given.</returns>
		vector<Shape*> solve3D(vector<Shape*> boxes) const;
};