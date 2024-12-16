#include "./glut/api/glutManager.hpp"
#include "./geometry/api/shapeBuilder.hpp"
#include <iostream>
#include <tuple>
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
		if (!(placedBoxes[i].first.x + placedBoxes[i].second.x <= box.first.x ||
				box.first.x + box.second.x <= placedBoxes[i].first.x ||
				placedBoxes[i].first.y + placedBoxes[i].second.x <= box.first.y ||
				box.first.y + box.second.y <= placedBoxes[i].first.y ||
				placedBoxes[i].first.z + placedBoxes[i].second.z <= box.first.z ||
				box.first.z + box.second.z <= placedBoxes[i].first.z
			))
		{
			return true;
		}
	}
	return false;
}

int main(int argc, char **argv) {

	vector<Shape*> scene;

	auto containerValues = ShapeBuilder::createBox(4.0f, 4.0f, 4.0f, vec4(1.0f));
	Shape* container = new Shape(containerValues.first, containerValues.second, 0.0f, 0.0f);
	scene.push_back(container);

	vector<Triple> boxes = {
		{1, 1, 1},
		{1, 2, 1},
		{4, 1, 1},
		{2, 1, 3},
		{2, 2, 3}
	};
	vector<pair<Triple, Triple>> placedBoxes;

	for (int i = 0; i < boxes.size(); i++) {
		int xi = 0;
		int yi = 0;
		int zi = 0;
		int maxx = maxX(placedBoxes, boxes[i]);
		int maxy = maxY(placedBoxes, boxes[i]);
		int maxz = maxZ(placedBoxes, boxes[i]);
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] }))
			xi = maxx;
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] })) {
			xi = 0;
			yi = maxy;
		}
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] }))
			xi = maxx;
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] })) {
			xi = 0;
			yi = 0;
			zi = maxz;
		}
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] }))
			xi = maxx;
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] })) {
			xi = 0;
			yi = maxy;
		}
		if (collides(placedBoxes, { { xi, yi, zi }, boxes[i] }))
			xi = maxx;
		placedBoxes.push_back({ { xi, yi, zi }, boxes[i] });
	}

	for (int i = 0; i < placedBoxes.size(); i++) {
		auto box = ShapeBuilder::createBox(placedBoxes[i].second.x, placedBoxes[i].second.y, placedBoxes[i].second.z, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Shape* shape = new Shape(box.first, box.second, 0.0f, 0.0f);
		shape->move(placedBoxes[i].first.x, placedBoxes[i].first.y, placedBoxes[i].first.z);
		scene.push_back(shape);
	}

	//for (int i = 0; i < placedBoxes.size(); i++)
	//	printf("x: %d\ty: %d\tz:%d\n", placedBoxes[i].first.x, placedBoxes[i].first.y, placedBoxes[i].first.z);

	GlutManager* glutManager = new GlutManager(scene);
	glutManager->openWindow(argc, argv);
}