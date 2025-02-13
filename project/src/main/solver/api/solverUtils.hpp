#pragma once
#include "../../geometry/api/box.hpp"

/// <summary>
/// Calculates the total weight of a vector of boxes.
/// </summary>
/// <param name="boxes"></param>
/// <returns></returns>
int getWeight(vector<Box*> boxes);

/// <summary>
/// Calculates the total profit of a vector of boxes.
/// </summary>
/// <param name="boxes"></param>
/// <returns></returns>
int getProfit(vector<Box*> boxes);

/// <summary>
/// Calculates the difference betweeen 2 vector of boxes.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>The boxes in v1 that are not in v2.</returns>
vector<Box*> getDifference(vector<Box*> v1, vector<Box*> v2);

/// <summary>
/// Checks if a box with the given coordinates would collide with another that is already placed.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="boxPosition"></param>
/// <param name="containerSize"></param>
/// <returns>true if the box collides with another one, false otherwise.</returns>
static bool collides(vector<Box*> placedBoxes, Box* box, vec3 boxPosition, vec3 containerSize);

/// <summary>
/// Calculates the x value of the rightmost placed box that would allow the current box to be placed on its right side.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
int getMaxX(vector<Box*> placedBoxes, Box* box, vec3 containerSize);

/// <summary>
/// Calculates the y value of the upmost placed box that would allow the current box to be placed above it.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
int getMaxY(vector<Box*> placedBoxes, Box* box, vec3 containerSize);

/// <summary>
/// Calculates the z value of the frontmost placed box that would allow the current box to be placed in front of it.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
int getMaxZ(vector<Box*> placedBoxes, Box* box, vec3 containerSize);

/// <summary>
/// Calculates all the available positions for a box.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
vector<vec3> getAvailablePositions(vector<Box*> placedBoxes, Box* box, vec3 containerSize);

/// <summary>
/// Calculates the best coordinates for a box.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
vec3 getCoordinates(vector<Box*> placedBoxes, Box* box, vec3 containerSize);

/// <summary>
/// Checks if a combination of boxes fit in a container.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="containerSize"></param>
/// <returns>true if the boxes fit, false otherwise.</returns>
bool fits(vector<Box*> placedBoxes, vec3 containerSize);

/// <summary>
/// Checks if a box doesn't have another box beneath it.
/// </summary>
/// <param name="box"></param>
/// <param name="position"></param>
/// <returns>True if the box doesn't have another box beneath it, false otherwise</returns>
bool isFlying(vector<Box*> placedBoxes, Box* box, vec3 position);

/// <summary>
/// Creates a random applicable solution.
/// </summary>
/// <param name="boxes"></param>
/// <param name="maxWeight"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
vector<Box*> createInitialSolution(vector<Box*> boxes, int maxWeight, vec3 containerSize);

/// <summary>
/// Creates a random neighbor solution.
/// </summary>
/// <param name="currentSolution"></param>
/// <param name="remainingBoxes"></param>
/// <returns></returns>
vector<Box*> createNeighborSolution(vector<Box*> currentSolution, vector<Box*> remainingBoxes);

/// <summary>
/// Creates the solution that matches a list of boxes.
/// </summary>
/// <param name="boxes"></param>
/// <returns></returns>
vector<pair<Box*, vec3>> createSolutionFromBoxes(vector<Box*> boxes);

vector<vec3> getBoxesCoordinates(vector<Box*> boxes);