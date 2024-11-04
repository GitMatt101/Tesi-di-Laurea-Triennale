#include "lib.hpp"
#include "shaders/api/shadersManager.hpp"

int main()
{
    ShadersManager* shaderManager = new ShadersManager((char*)"../shaderFiles/vertexShader.glsl", (char*)"../shaderFiles/fragmentShader.glsl");
}