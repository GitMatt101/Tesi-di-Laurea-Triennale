#pragma once
#include <vector>
#include "../../geometry/api/box.hpp"
using namespace std;

class KnapsackSolver {
	private:
		vec3 containerSize;
		float maxWeight;

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		/// <param name="values"></param>
		KnapsackSolver(vec3 containerSize, float maxWeight);

		/// <summary>
		/// Solves 3-dimensional Knapsack Problem.
		/// </summary>
		/// <param name="boxes"></param>
		/// <returns>A vector of boxes chosen among the ones given.</returns>
		vector<pair<Box*, vec3>> solve3D(vector<Box*> boxes) const;
};