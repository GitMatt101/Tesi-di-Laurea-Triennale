#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include <vector>
#include "../../geometry/api/shape.hpp"
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
		vector<Shape*> shapes;
		Camera* camera;
		ShadersManager* shadersManager;

		/// <summary>
		/// Draws the scene.
		/// </summary>
		void drawScene(void);

		/// <summary>
		/// Moves the camera.
		/// </summary>
		/// <param name="x">- Horizontal movement.</param>
		/// <param name="y">- Vertical movement.</param>
		/// <param name="z">- Depth movement.</param>
		void moveCamera(float x, float y, float z);

		/// <summary>
		/// Zooms in or out.
		/// </summary>
		/// <param name="direction">- Direction of the wheel: +1 forward, -1 backward.</param>
		void zoomCamera(int direction);

		/// <summary>
		/// Looks around.
		/// </summary>
		/// <param name="x">- Horizontal position of the cursor.</param>
		/// <param name="y">- Verrtical position of the cursor.</param>
		void lookAround(int x, int z);

		/// <summary>
		/// Glut callback function to relaod the window.
		/// </summary>
		/// <param name="value"></param>
		static void update(int value);

	public:
		/// <summary>
		/// Base constructor
		/// </summary>
		GlutManager();

		/// <summary>
		/// Opens a window to run the application on.
		/// </summary>
		/// <param name="argc">Number of arguments passed to main on application's execution.</param>
		/// <param name="argv">Arguments passed to main on application's execution.<param>
		void openWindow(int argc, char** argv);

		/// <summary>
		/// Accessor for the draw scene flut function.
		/// </summary>
		static void drawSceneAccessor(void);

		/// <summary>
		/// Accessor for the keyboard glut function.
		/// </summary>
		/// <param name="key">- ASCII code of the pressed key.</param>
		/// <param name="x">- Horizontal position of the cursor when the key is pressed.</param>
		/// <param name="y">- Verrtical position of the cursor when the key is pressed.</param>
		static void movementAccessor(unsigned char key, int x, int y);

		/// <summary>
		/// Accessor for the special keys glut function.
		/// </summary>
		/// <param name="key">- ASCII code of the pressed key.</param>
		/// <param name="x">- Horizontal position of the cursor when the key is pressed.</param>
		/// <param name="y">- Verrtical position of the cursor when the key is pressed.</param>
		static void specialKeyAccessor(int key, int x, int y);

		/// <summary>
		/// Accessor for the zoom glut function.
		/// </summary>
		/// <param name="wheel">- Number of the mouse wheel.</param>
		/// <param name="direction">- Direction of the mouse wheel: +1 forward, -1 backward.</param>
		/// <param name="x">- Horizontal position of the cursor when the wheel is used.</param>
		/// <param name="y">- Verrtical position of the cursor when the wheel is used.</param>
		static void zoomAccessor(int wheel, int direction, int x, int y);

		/// <summary>
		/// Accessor for the passive mouse movement glut function.
		/// </summary>
		/// <param name="x">- Horizontal position of the cursor.</param>
		/// <param name="y">- Verrtical position of the cursor.</param>
		static void lookAroundAccessor(int x, int y);
};