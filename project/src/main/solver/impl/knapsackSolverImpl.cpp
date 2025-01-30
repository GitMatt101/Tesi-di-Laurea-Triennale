#include "../api/knapsackSolver.hpp"
#include <iostream>
#include <algorithm>
#include <map>

constexpr auto MAX_ITERATIONS = 10000;

static int getWeight(vector<Shape*> boxes) {
	int weight = 0;
	for (Shape* box : boxes)
		weight += box->getWeight();
	return weight;
}

static int getProfit(vector<Shape*> boxes) {
	int profit = 0;
	for (Shape* box : boxes)
		profit += box->getValue();
	return profit;
}

static vector<Shape*> getDifference(vector<Shape*> s1, vector<Shape*> s2) {
	sort(s1.begin(), s1.end());
	sort(s2.begin(), s2.end());
	vector<Shape*> diff;
	set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(diff));
	return diff;
}

static bool collides(vector<Shape*> placedBoxes, Shape* box, vec3 boxPosition, vec3 containerSize) {
	for (Shape* boxPlaced : placedBoxes) {
		bool xCheck = boxPlaced->getPosition().x + boxPlaced->getSize().x <= boxPosition.x || boxPosition.x + box->getSize().x <= boxPlaced->getPosition().x;
		bool yCheck = boxPlaced->getPosition().y + boxPlaced->getSize().y <= boxPosition.y || boxPosition.y + box->getSize().y <= boxPlaced->getPosition().y;
		bool zCheck = boxPlaced->getPosition().z + boxPlaced->getSize().z <= boxPosition.z || boxPosition.z + box->getSize().z <= boxPlaced->getPosition().z;
		bool collision = !(xCheck || yCheck || zCheck);
		if (collision || boxPosition.x + box->getSize().x > containerSize.x || boxPosition.y + box->getSize().y > containerSize.y || boxPosition.z + box->getSize().z > containerSize.z)
			return true;
	}
	return false;
}

static int getMaxX(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
	vector<Shape*> px;
	for (Shape* placedBox : placedBoxes) {
		if (placedBox->getPosition().x + placedBox->getSize().x <= containerSize.x - box->getSize().x)
			px.push_back(placedBox);
	}
	int maxX = 0;
	for (int i = 0; i < px.size(); i++) {
		int x = px[i]->getPosition().x + px[i]->getSize().x;
		if (x > maxX)
			maxX = x;
	}
	return maxX;
}

static int getMaxY(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
	vector<Shape*> py;
	for (Shape* placedBox : placedBoxes) {
		if (placedBox->getPosition().y + placedBox->getSize().y <= containerSize.y - box->getSize().y)
			py.push_back(placedBox);
	}
	int maxY = 0;
	for (int i = 0; i < py.size(); i++) {
		int y = py[i]->getPosition().y + py[i]->getSize().y;
		if (y > maxY)
			maxY = y;
	}
	return maxY;
}

static int getMaxZ(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
	vector<Shape*> pz;
	for (Shape* placedBox : placedBoxes) {
		if (placedBox->getPosition().z + placedBox->getSize().z <= containerSize.z - box->getSize().z)
			pz.push_back(placedBox);
	}
	int maxZ = 0;
	for (int i = 0; i < pz.size(); i++) {
		int z = pz[i]->getPosition().z + pz[i]->getSize().z;
		if (z > maxZ)
			maxZ = z;
	}
	return maxZ;
}

vec3 getCoordinates(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
	if (placedBoxes.size() == 0)
		return vec3(0);

	vector<vec3> availablePositions;
	for (Shape* placedBox : placedBoxes) {
		int xi = placedBox->getPosition().x;
		int xRight = xi + placedBox->getSize().x;
		int yi = placedBox->getPosition().y;
		int yUp = yi + placedBox->getSize().y;
		int zi = placedBox->getPosition().z;
		int zFront = zi + placedBox->getSize().z;
		if (!collides(placedBoxes, box, vec3(xRight, yi, zi), containerSize))
			availablePositions.push_back(vec3(xRight, yi, zi));
		if (!collides(placedBoxes, box, vec3(xi, yUp, zi), containerSize))
			availablePositions.push_back(vec3(xi, yUp, zi));
		if (!collides(placedBoxes, box, vec3(xi, yi, zFront), containerSize))
			availablePositions.push_back(vec3(xi, yi, zFront));
	}
	int valuesA[] = { 0, getMaxX(placedBoxes, box, containerSize) };
	int valuesB[] = { 0, getMaxY(placedBoxes, box, containerSize) };
	int valuesC[] = { 0, getMaxZ(placedBoxes, box, containerSize) };
	for (int j = 0; j < 2; ++j) {
		for (int k = 0; k < 2; ++k) {
			for (int l = 0; l < 2; ++l) {
				int x = valuesA[j];
				int y = valuesB[k];
				int z = valuesC[l];
				if (!collides(placedBoxes, box, vec3(x, y, z), containerSize))
					availablePositions.push_back(vec3(x, y, z));
			}
		}
	}
	if (availablePositions.size() == 0)
		return vec3(-1);

	// Sort by depth
	sort(availablePositions.begin(), availablePositions.end(), [](vec3 a, vec3 b) { return a.z < b.z; });
	map<int, vector<vec3>> positions;
	for (vec3 position : availablePositions)
		positions[position.z].push_back(position);

	vector<vec3> bottomPositions = positions[availablePositions[0].z];
	positions = {};
	if (bottomPositions.size() == 1)
		return bottomPositions[0];

	// Sort by vertical position
	sort(bottomPositions.begin(), bottomPositions.end(), [](vec3 a, vec3 b) { return a.y < b.y; });
	for (vec3 position : bottomPositions)
		positions[position.y].push_back(position);
	vector<vec3> backPositions = positions[bottomPositions[0].y];
	positions = {};
	if (backPositions.size() == 1)
		return backPositions[0];

	// Sort by horizontal position
	sort(backPositions.begin(), backPositions.end(), [](vec3 a, vec3 b) { return a.x < b.x; });
	for (vec3 position : backPositions)
		positions[position.x].push_back(position);
	vector<vec3> leftPositions = positions[backPositions[0].x];
	return leftPositions[0];
}

static bool fits(vector<Shape*> placedBoxes, vec3 containerSize) {
	vector<Shape*> boxes;
	for (Shape* box : placedBoxes) {
		vec3 c = getCoordinates(boxes, box, vec3(9.0f, 9.0f, 9.0f));
		if (c == vec3(-1))
			return false;
		vec3 boxSize = box->getSize();
		box->setPosition(c);
		boxes.push_back(box);
	}
	return true;
}

KnapsackSolver::KnapsackSolver(float width, float height, float depth, float maxWeight) {
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->maxWeight = maxWeight;
	this->emptySpace = tuple<float, float, float>(width, height, depth);
}

vector<Shape*> KnapsackSolver::solve3D(vector<Shape*> boxes) const {
	vector<Shape*> initialSolution;
	do {
		initialSolution.clear();
		for (Shape* box : boxes) {
			if (rand() % 2 == 1)
				initialSolution.push_back(box);
		}
	} while (getWeight(initialSolution) > this->maxWeight || !fits(initialSolution, vec3(this->width, this->height, this->depth)));

	vector<vector<Shape*>> solutions;
	solutions.push_back(initialSolution);
	for (int i = MAX_ITERATIONS; i > 0; i--) {
		vector<Shape*> neighbor = initialSolution;
		vector<Shape*> boxesLeft = getDifference(boxes, neighbor);
		if (boxesLeft.size() == 0) {
			if (getWeight(neighbor) <= this->maxWeight && fits(neighbor, vec3(this->width, this->height, this->depth)))
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
		auto it = find(solutions.begin(), solutions.end(), neighbor);
		if (it != solutions.end())
			continue;
		int initialValue = getProfit(initialSolution);
		int neighborValue = getProfit(neighbor);
		bool accept = false;
		if (neighborValue >= initialValue) {
			if (fits(neighbor, vec3(this->width, this->height, this->depth)))
				accept = true;
		} else {
			float p = rand() % 1001 / 1000.0f;
			int delta = neighborValue - initialValue;
			if (exp(delta / i) > p && fits(neighbor, vec3(this->width, this->height, this->depth)))
				accept = true;
		}
		if (accept) {
			initialSolution = neighbor;
			solutions.push_back(neighbor);
		}
	}
	vector<Shape*> max;
	for (vector<Shape*> solution : solutions) {
		if (getProfit(solution) > getProfit(max))
			max = solution;
	}
	return max;
}