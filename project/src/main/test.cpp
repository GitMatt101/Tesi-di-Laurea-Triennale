#include "./glut/api/glutManager.hpp"
#include "./geometry/api/shapeBuilder.hpp"
#include <iostream>
#include <tuple>
#include <time.h>
#include <algorithm>
#include <map>
using namespace std;

struct Triple {
	int x;
	int y;
	int z;
};

static int maxX(vector<pair<Triple, Triple>> placedBoxes, Triple box) {
	vector<pair<Triple, Triple>> px;
	for (int i = 0; i < placedBoxes.size(); i++) {
		if (placedBoxes[i].first.x + placedBoxes[i].second.x <= 4 - box.x)
			px.push_back(placedBoxes[i]);
	}
	int maxX = 0;
	for (int i = 0; i < px.size(); i++) {
		int x = px[i].first.x + px[i].second.x;
		if (x > maxX)
			maxX = x;
	}
	return maxX;
}

static int maxY(vector<pair<Triple, Triple>> placedBoxes, Triple box) {
	vector<pair<Triple, Triple>> py;
	for (int i = 0; i < placedBoxes.size(); i++) {
		if (placedBoxes[i].first.y + placedBoxes[i].second.y <= 4 - box.y)
			py.push_back(placedBoxes[i]);
	}
	int maxY = 0;
	for (int i = 0; i < py.size(); i++) {
		int y = py[i].first.y + py[i].second.y;
		if (y > maxY)
			maxY = y;
	}
	return maxY;
}

static int maxZ(vector<pair<Triple, Triple>> placedBoxes, Triple box) {
	vector<pair<Triple, Triple>> pz;
	for (int i = 0; i < placedBoxes.size(); i++) {
		if (placedBoxes[i].first.z + placedBoxes[i].second.z <= 4 - box.z)
			pz.push_back(placedBoxes[i]);
	}
	int maxZ = 0;
	for (int i = 0; i < pz.size(); i++) {
		int z = pz[i].first.z + pz[i].second.z;
		if (z > maxZ)
			maxZ = z;
	}
	return maxZ;
}

static bool collides(vector<pair<Triple, Triple>> placedBoxes, pair<Triple, Triple> box) {
	for (int i = 0; i < placedBoxes.size(); i++) {
		bool xCheck = placedBoxes[i].first.x + placedBoxes[i].second.x <= box.first.x || box.first.x + box.second.x <= placedBoxes[i].first.x;
		bool yCheck = placedBoxes[i].first.y + placedBoxes[i].second.y <= box.first.y || box.first.y + box.second.y <= placedBoxes[i].first.y;
		bool zCheck = placedBoxes[i].first.z + placedBoxes[i].second.z <= box.first.z || box.first.z + box.second.z <= placedBoxes[i].first.z;
		bool collision = !(xCheck || yCheck || zCheck);
		if (collision || box.first.x + box.second.x > 4 || box.first.y + box.second.y > 4 || box.first.z + box.second.z > 4)
			return true;
	}
	return false;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	vector<Shape*> scene;

	auto containerValues = ShapeBuilder::createBox(4.0f, 4.0f, 4.0f, vec4(1.0f));
	Shape* container = new Shape(containerValues.first, containerValues.second, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	scene.push_back(container);

	vector<Triple> boxes = {
		{ 1, 3, 2 },
		{ 3, 2, 1 },
		{ 4, 1, 1 },
		{ 1, 1, 1 },
		{ 2, 2, 2 },
		{ 1, 2, 1 },
		{ 1, 1, 2 },
		{ 1, 2, 2 },
		{ 3, 3, 1 },
		{ 1, 2, 2 },
		{ 1, 2, 2 }
	};
	vector<pair<Triple, Triple>> placedBoxes;
	vector<pair<Triple, Triple>> algoBoxes;

	for (int i = 0; i < boxes.size(); i++) {
		vector<Triple> availablePositions;
		if (placedBoxes.size() == 0) {
			placedBoxes.push_back({ { 0, 0, 0 }, boxes[i] });
			algoBoxes.push_back({ { 0, 0, 0 }, boxes[i] });
			continue;
		}
		for (int j = 0; j < algoBoxes.size(); j++) {
			int xi = algoBoxes[j].first.x;
			int xRight = algoBoxes[j].first.x + algoBoxes[j].second.x;
			int yi = algoBoxes[j].first.y;
			int yUp = algoBoxes[j].first.y + algoBoxes[j].second.y;
			int zi = algoBoxes[j].first.z;
			int zFront = algoBoxes[j].first.z + algoBoxes[j].second.z;
			if (!collides(placedBoxes, { { xRight, yi, zi }, boxes[i] }))
				availablePositions.push_back({ xRight, yi, zi });
			if (!collides(placedBoxes, { { xi, yUp, zi }, boxes[i] }))
				availablePositions.push_back({ xi, yUp, zi });
			if (!collides(placedBoxes, { { xi, yi, zFront }, boxes[i] }))
				availablePositions.push_back({ xi, yi, zFront });
		}
		if (availablePositions.size() == 0) {
			int valuesA[] = { 0, maxX(placedBoxes, boxes[i]) };
			int valuesB[] = { 0, maxY(placedBoxes, boxes[i]) };
			int valuesC[] = { 0, maxZ(placedBoxes, boxes[i]) };
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					for (int l = 0; l < 2; ++l) {
						int x = valuesA[j];
						int y = valuesB[k];
						int z = valuesC[l];
						if (!collides(placedBoxes, { { x, y, z }, boxes[i] }))
							availablePositions.push_back({ x, y, z });
					}
				}
			}
			if (availablePositions.size() == 0)
				printf("NO SPACE LEFT FOR BOX %dx%dx%d\n", boxes[i].x, boxes[i].y, boxes[i].z);
		}
		sort(availablePositions.begin(), availablePositions.end(), [](Triple a, Triple b) { return a.z < b.z; });
		map<int, vector<Triple>> positions;
		for (Triple position : availablePositions)
			positions[position.z].push_back(position);

		vector<Triple> bottomPositions = positions[availablePositions[0].z];
		positions = {};
		if (bottomPositions.size() > 1) {
			sort(bottomPositions.begin(), bottomPositions.end(), [](Triple a, Triple b) { return a.y < b.y; });
			for (Triple position : bottomPositions)
				positions[position.y].push_back(position);
			vector<Triple> backPositions = positions[bottomPositions[0].y];
			positions = {};
			if (backPositions.size() > 1) {
				sort(backPositions.begin(), backPositions.end(), [](Triple a, Triple b) { return a.x < b.x; });
				for (Triple position : backPositions)
					positions[position.x].push_back(position);
				vector<Triple> leftPositions = positions[backPositions[0].x];
				positions = {};
				placedBoxes.push_back({ leftPositions[0], boxes[i]});
				algoBoxes.push_back({ leftPositions[0], boxes[i] });
			} else {
				placedBoxes.push_back({ backPositions[0], boxes[i] });
				algoBoxes.push_back({ backPositions[0], boxes[i] });
			}
		}
		else {
			placedBoxes.push_back({ bottomPositions[0], boxes[i] });
			algoBoxes.push_back({ bottomPositions[0], boxes[i] });
		}

		auto it = algoBoxes.begin();
		while (it != algoBoxes.end()) {
			auto& box = *it;
			Triple right = { box.first.x + box.second.x, box.first.y, box.first.z };
			Triple up = { box.first.x, box.first.y + box.second.y, box.first.z };
			Triple front = { box.first.x, box.first.y, box.first.z + box.second.z };
			bool bRight = right.x == 4;
			bool bUp = up.y == 4;
			bool bFront = front.z == 4;
			for (const pair<Triple, Triple> aBox : algoBoxes) {
				if (aBox.first.x == right.x 
					&& aBox.first.y <= right.y && aBox.first.y + aBox.second.y > right.y
					&& aBox.first.z <= right.z && aBox.first.z + aBox.second.z > right.z)
					bRight = true;
				if (aBox.first.x <= up.x && aBox.first.x + aBox.second.x > up.x
					&& aBox.first.y == up.y
					&& aBox.first.z <= up.z && aBox.first.z + aBox.second.z > up.z)
					bUp = true;
				if (aBox.first.x <= front.x && aBox.first.x + aBox.second.x > front.x
					&& aBox.first.y <= front.y && aBox.first.y + aBox.second.y > front.y
					&& aBox.first.z >= front.z) {
					bFront = true;
					if (aBox.first.x + aBox.second.x > box.first.x + box.second.x)
						bRight = true;
					if (aBox.first.y + aBox.second.y > box.first.y + box.second.y)
						bUp = true;
				}
			}
			if (bRight && bUp && bFront)
				it = algoBoxes.erase(it);
			else
				++it;
		}
	}

	for (int i = 0; i < placedBoxes.size(); i++) {
		int val = rand() % 15 + 10;
		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		auto box = ShapeBuilder::createBox(placedBoxes[i].second.x, placedBoxes[i].second.y, placedBoxes[i].second.z, vec4(r, g, b, 1.0f));
		Shape* shape = new Shape(
			box.first,
			box.second,
			0.0f, 0.0f,
			0, 0, val,
			placedBoxes[i].first.x, placedBoxes[i].first.y, placedBoxes[i].first.z
		);
		scene.push_back(shape);
	}

	GlutManager* glutManager = new GlutManager(scene);
	glutManager->openWindow(argc, argv);
}