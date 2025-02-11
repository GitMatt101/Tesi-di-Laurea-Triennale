#pragma once
#include "../../geometry/api/shape.hpp"

/// <summary>
/// Calculates the total weight of a vector of shapes.
/// </summary>
/// <param name="boxes"></param>
/// <returns></returns>
int getWeight(vector<Shape*> boxes);

/// <summary>
/// Calculates the total profit of a vector of shapes.
/// </summary>
/// <param name="boxes"></param>
/// <returns></returns>
int getProfit(vector<Shape*> boxes);

/// <summary>
/// Calculates the difference betweeen 2 vector of shapes.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>The shapes in v1 that are not in v2.</returns>
vector<Shape*> getDifference(vector<Shape*> v1, vector<Shape*> v2);

/// <summary>
/// Checks if a box with the given coordinates would collide with another that is already placed.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="boxPosition"></param>
/// <param name="containerSize"></param>
/// <returns>true if the box collides with another one, false otherwise.</returns>
static bool collides(vector<Shape*> placedBoxes, Shape* box, vec3 boxPosition, vec3 containerSize);

/// <summary>
/// Calculates the x value of the rightmost placed box that would allow the current box to be placed on its right side.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
int getMaxX(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize);

/// <summary>
/// Calculates the y value of the upmost placed box that would allow the current box to be placed above it.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
int getMaxY(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize);

/// <summary>
/// Calculates the z value of the frontmost placed box that would allow the current box to be placed in front of it.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
int getMaxZ(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize);


/// <summary>
/// Calculates the best coordinates for a box.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="box"></param>
/// <param name="containerSize"></param>
/// <returns></returns>
vec3 getCoordinates(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize);

/// <summary>
/// Checks if a combination of boxes fit in a container.
/// </summary>
/// <param name="placedBoxes"></param>
/// <param name="containerSize"></param>
/// <returns>true if the boxes fit, false otherwise.</returns>
bool fits(vector<Shape*> placedBoxes, vec3 containerSize);