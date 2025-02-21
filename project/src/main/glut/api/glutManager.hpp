#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include <vector>
#include "../../geometry/api/box.hpp"
#include "../../camera/api/camera.hpp"
#include "../../shaders/api/shadersManager.hpp"
using namespace std;
using namespace glm;

class GlutManager {
	private:
		static GlutManager* instance;

		unsigned int projectionMatrixUniform;
		unsigned int viewMatrixUniform;
		unsigned int modelMatrixUniform;
		unsigned int viewPositionUniform;
		mat4 projectionMatrix;
		mat4 viewMatrix;
		vector<Box*> boxes;
		Camera* camera;
		ShadersManager* shadersManager;
		GLenum polygonMode;
		GLenum elementsMode;
		bool pause;
		bool insert;

		/// <summary>
		/// Draws the scene.
		/// </summary>
		static void drawScene(void);

		/// <summary>
		/// Moves the camera.
		/// </summary>
		/// <param name="key">- Key pressed.</param>
		/// <param name="x">- Horizontal position of the cursors when the key is pressed.</param>
		/// <param name="y">- Vertical position of the cursors when the key is pressed.</param>
		static void moveCamera(unsigned char key, int x, int y);

		/// <summary>
		/// Zooms in and out.
		/// </summary>
		/// <param name="wheel"></param>
		/// <param name="direction">- Direction of the mouse wheel (+1 forward. -1 backward)</param>
		/// <param name="x">- Horizontal position of the cursors when the key is pressed.</param>
		/// <param name="y">- Vertical position of the cursors when the key is pressed.</param>
		static void zoomCamera(int wheel, int direction, int x, int y);

		/// <summary>
		/// Looks around.
		/// </summary>
		/// <param name="x">- Horizontal position of the cursor.</param>
		/// <param name="y">- Verrtical position of the cursor.</param>
		static void lookAround(int x, int z);

		/// <summary>
		/// Glut callback function to relaod the window.
		/// </summary>
		/// <param name="value"></param>
		static void update(int value);

	public:
		/// <summary>
		/// Base constructor.
		/// </summary>
		/// <param name="boxes">Boxes to show in the view.</param>
		GlutManager(vector<Box*> boxes);

		/// <summary>
		/// Opens a window to run the application on.
		/// </summary>
		/// <param name="argc">Number of arguments passed to main on application's execution.</param>
		/// <param name="argv">Arguments passed to main on application's execution.<param>
		void openWindow(int argc, char** argv);
};