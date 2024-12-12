#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include <vector>
#include "../../geometry/api/shape.hpp"
using namespace std;
using namespace glm;

class GlutManager {
	private:
		static GlutManager* instance;

		unsigned int programId;
		unsigned int projectionMatrixUniform;
		unsigned int viewMatrixUniform;
		unsigned int modelMatrixUniform;
		mat4 projectionMatrix;
		mat4 viewMatrix;
		vector<Shape*> shapes;

		/// <summary>
		/// Draws the scene.
		/// </summary>
		void drawScene(void);

		/// <summary>
		/// Draws the scene.
		/// </summary>
		static void drawSceneAccessor(void);

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		GlutManager(unsigned int programId, vector<Shape*> shapes);

		/// <summary>
		/// Opens a window to run the application on.
		/// </summary>
		/// <param name="argc">Number of arguments passed to main on application's execution.</param>
		/// <param name="argv">Arguments passed to main on application's execution.<param>
		void openWindow(int argc, char** argv);
};