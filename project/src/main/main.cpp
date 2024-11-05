#include "lib.hpp"
#include "shaders/api/shadersManager.hpp"
#include "geometry/api/shapes.hpp"
#include "geometry/api/geometry.hpp"

int main()
{
    ShadersManager* shaderManager = new ShadersManager((char*)"../shaderFiles/vertexShader.glsl", (char*)"../shaderFiles/fragmentShader.glsl");
}