#pragma once
#include "../../geometry/api/shapes.hpp"

/**
* Initializes a shape's Vertex Array Object and Vertex Buffer Objects.
*
* @param shape - shape to create VAO and VBOs for.
*/
void initShapeVAO(Shape* shape);

/**
* Creates a Vertex Array Object and a Vertex Buffer Object for text display.
*
* @param textVAO - pointer to the text VAO.
* @param textVBO - pointer to the text VBO.
*/
void initTextVAO(GLuint* textVAO, GLuint* textVBO);