#include "../api/solverUtils.hpp"
#include <algorithm>
#include <iostream>
#include <map>

int getWeight(vector<Shape*> boxes) {
	int weight = 0;
	for (Shape* box : boxes)
		weight += box->getWeight();
	return weight;
}
int getProfit(vector<Shape*> boxes) {
	int profit = 0;
	for (Shape* box : boxes)
		profit += box->getValue();
	return profit;
}

vector<Shape*> getDifference(vector<Shape*> v1, vector<Shape*> v2) {
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	vector<Shape*> diff;
	set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(diff));
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

int getMaxX(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
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

int getMaxY(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
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

int getMaxZ(vector<Shape*> placedBoxes, Shape* box, vec3 containerSize) {
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
	if (availablePositions.size() == 0) {
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

bool fits(vector<Shape*> placedBoxes, vec3 containerSize) {
	vector<Shape*> boxes;
	for (Shape* box : placedBoxes) {
		vec3 c = getCoordinates(boxes, box, containerSize);
		if (c == vec3(-1))
			return false;
		vec3 boxSize = box->getSize();
		box->setPosition(c);
		boxes.push_back(box);
	}
	return true;
}