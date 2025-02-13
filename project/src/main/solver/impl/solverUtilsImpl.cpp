#include "../api/solverUtils.hpp"
#include <algorithm>
#include <iostream>
#include <map>

int getWeight(vector<Box*> boxes) {
	int weight = 0;
	for (Box* box : boxes)
		weight += box->getWeight();
	return weight;
}

int getProfit(vector<Box*> boxes) {
	int profit = 0;
	for (Box* box : boxes)
		profit += box->getValue();
	return profit;
}

vector<Box*> getDifference(vector<Box*> v1, vector<Box*> v2) {
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	vector<Box*> diff;
	set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(diff));
	return diff;
}

static bool collides(vector<Box*> placedBoxes, Box* box, vec3 boxPosition, vec3 containerSize) {
	for (Box* placedBox : placedBoxes) {
		vec3 pbPos = placedBox->getPosition();
		vec3 pbSize = placedBox->getSize();
		bool xCheck = pbPos.x + pbSize.x <= boxPosition.x || boxPosition.x + box->getSize().x <= pbPos.x;
		bool yCheck = pbPos.y + pbSize.y <= boxPosition.y || boxPosition.y + box->getSize().y <= pbPos.y;
		bool zCheck = pbPos.z + pbSize.z <= boxPosition.z || boxPosition.z + box->getSize().z <= pbPos.z;
		bool collision = !(xCheck || yCheck || zCheck);
		if (collision || boxPosition.x + box->getSize().x > containerSize.x || boxPosition.y + box->getSize().y > containerSize.y || boxPosition.z + box->getSize().z > containerSize.z)
			return true;
	}
	return false;
}

int getMaxX(vector<Box*> placedBoxes, Box* box, vec3 containerSize) {
	vector<Box*> px;
	std::copy_if(placedBoxes.begin(), placedBoxes.end(), std::back_inserter(px), [&containerSize](Box* pBox) {
		return pBox->getPosition().x + pBox->getSize().x <= containerSize.x - pBox->getSize().x;
	});
	int maxX = 0;
	for (int i = 0; i < px.size(); i++) {
		int x = px[i]->getPosition().x + px[i]->getSize().x;
		if (x > maxX)
			maxX = x;
	}
	return maxX;
}

int getMaxY(vector<Box*> placedBoxes, Box* box, vec3 containerSize) {
	vector<Box*> py;
	std::copy_if(placedBoxes.begin(), placedBoxes.end(), std::back_inserter(py), [&containerSize](Box* pBox) {
		return pBox->getPosition().y + pBox->getSize().y <= containerSize.y - pBox->getSize().y;
	});
	int maxY = 0;
	for (int i = 0; i < py.size(); i++) {
		int y = py[i]->getPosition().y + py[i]->getSize().y;
		if (y > maxY)
			maxY = y;
	}
	return maxY;
}

int getMaxZ(vector<Box*> placedBoxes, Box* box, vec3 containerSize) {
	vector<Box*> pz;
	std::copy_if(placedBoxes.begin(), placedBoxes.end(), std::back_inserter(pz), [&containerSize](Box* pBox) {
		return pBox->getPosition().z + pBox->getSize().z <= containerSize.z - pBox->getSize().z;
	});
	int maxZ = 0;
	for (int i = 0; i < pz.size(); i++) {
		int z = pz[i]->getPosition().z + pz[i]->getSize().z;
		if (z > maxZ)
			maxZ = z;
	}
	return maxZ;
}

vec3 getCoordinates(vector<Box*> placedBoxes, Box* box, vec3 containerSize) {
	if (placedBoxes.size() == 0)
		return vec3(0);

	vector<vec3> positions = getAvailablePositions(placedBoxes, box, containerSize);
	if (positions.size() == 0)
		return vec3(-1);

	// Sort by depth
	sort(positions.begin(), positions.end(), [](vec3 a, vec3 b) { return a.z < b.z; });
	map<int, vector<vec3>> coordinatesMap;
	for (vec3 position : positions)
		coordinatesMap[position.z].push_back(position);

	vector<vec3> bottomPositions = coordinatesMap[positions[0].z];
	coordinatesMap = {};
	if (bottomPositions.size() == 1)
		return bottomPositions[0];

	// Sort by vertical position
	sort(bottomPositions.begin(), bottomPositions.end(), [](vec3 a, vec3 b) { return a.y < b.y; });
	for (vec3 position : bottomPositions)
		coordinatesMap[position.y].push_back(position);
	vector<vec3> backPositions = coordinatesMap[bottomPositions[0].y];
	coordinatesMap = {};
	if (backPositions.size() == 1)
		return backPositions[0];

	// Sort by horizontal position
	sort(backPositions.begin(), backPositions.end(), [](vec3 a, vec3 b) { return a.x < b.x; });
	for (vec3 position : backPositions)
		coordinatesMap[position.x].push_back(position);
	vector<vec3> leftPositions = coordinatesMap[backPositions[0].x];
	return leftPositions[0];
}

vector<vec3> getAvailablePositions(vector<Box*> placedBoxes, Box* box, vec3 containerSize) {
	vector<vec3> positions;
	for (Box* placedBox : placedBoxes) {
		int xi = placedBox->getPosition().x;
		int xRight = xi + placedBox->getSize().x;
		int yi = placedBox->getPosition().y;
		int yUp = yi + placedBox->getSize().y;
		int zi = placedBox->getPosition().z;
		int zFront = zi + placedBox->getSize().z;
		if (!collides(placedBoxes, box, vec3(xRight, yi, zi), containerSize) && !isFlying(placedBoxes, box, vec3(xRight, yi, zi)))
			positions.push_back(vec3(xRight, yi, zi));
		if (!collides(placedBoxes, box, vec3(xi, yUp, zi), containerSize) && !isFlying(placedBoxes, box, vec3(xRight, yi, zi)))
			positions.push_back(vec3(xi, yUp, zi));
		if (!collides(placedBoxes, box, vec3(xi, yi, zFront), containerSize) && !isFlying(placedBoxes, box, vec3(xi, yi, zFront)))
			positions.push_back(vec3(xi, yi, zFront));
	}
	int xValues[] = { 0, getMaxX(placedBoxes, box, containerSize) };
	int yValues[] = { 0, getMaxY(placedBoxes, box, containerSize) };
	int zValues[] = { 0, getMaxZ(placedBoxes, box, containerSize) };
	for (int j = 0; j < 2; ++j) {
		for (int k = 0; k < 2; ++k) {
			for (int l = 0; l < 2; ++l) {
				int x = xValues[j];
				int y = yValues[k];
				int z = zValues[l];
				if (!collides(placedBoxes, box, vec3(x, y, z), containerSize) && !isFlying(placedBoxes, box, vec3(x, y, z)))
					positions.push_back(vec3(x, y, z));
			}
		}
	}
	return positions;
}

bool fits(vector<Box*> placedBoxes, vec3 containerSize) {
	vector<Box*> boxes;
	for (Box* box : placedBoxes) {
		vec3 c = getCoordinates(boxes, box, containerSize);
		if (c == vec3(-1))
			return false;
		box->setPosition(c);
		boxes.push_back(box);
	}
	return true;
}

bool isFlying(vector<Box*> placedBoxes, Box* box, vec3 position) {
	if (position.y == 0)
		return false;
	vec3 size = box->getSize();
	vec3 midPoint = vec3(position.x + size.x / 2, position.y, position.z + size.z / 2);
	for (Box* placedBox : placedBoxes) {
		vec3 pbPos = placedBox->getPosition();
		vec3 pbSize = placedBox->getSize();
		if (pbPos.y + pbSize.y == midPoint.y
			&& pbPos.x <= midPoint.x && midPoint.x <= pbPos.x + pbSize.x
			&& pbPos.z <= midPoint.z && midPoint.z <= pbPos.z + pbSize.z) {
			return false;
		}
	}
	return true;
}

vector<Box*> createInitialSolution(vector<Box*> boxes, int maxWeight, vec3 containerSize) {
	vector<Box*> solution;
	do {
		solution.clear();
		for (Box* box : boxes) {
			if (rand() % 2 == 1)
				solution.push_back(box);
		}
	} while (getWeight(solution) > maxWeight || !fits(solution, containerSize));
	return solution;
}

vector<Box*> createNeighborSolution(vector<Box*> currentSolution, vector<Box*> remainingBoxes) {
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

vector<vec3> getBoxesCoordinates(vector<Box*> boxes) {
	vector<vec3> coords;
	for (Box* box : boxes)
		coords.push_back(box->getPosition());
	return coords;
}

vector<pair<Box*, vec3>> createSolutionFromBoxes(vector<Box*> boxes) {
	vector<pair<Box*, vec3>> solution;
	for (Box* box : boxes)
		solution.push_back({ box, box->getPosition() });
	return solution;
}