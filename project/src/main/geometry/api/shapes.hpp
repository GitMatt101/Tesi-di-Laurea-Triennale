#pragma once
#include "geometry.hpp"

class Shape {
	private:
		GLuint shapeVAO;
		GLuint verticesVBO;
		GLuint colorsVBO;
		mat4 model;
		vector<Vertex> vertices;
		float x;
		float y;
		float rotationAngle;

	public:
		/**
		* Basic constructor that initializes all the fields and creates the shape.
		* 
		* @param vertices - vertices of the shape.
		*/
		Shape(vector<Vertex> vertices);

		/**
		* Initializes the shape's Vertex Array Object.
		*/
		void initVAO();

		/**
		* @return the pointer to the shape's Vertex Array Object.
		*/
		GLuint* getVAO();

		/**
		* @return the pointer to the shape's vertices' Vertex Buffer Object vertices.
		*/
		GLuint* getVerticesVBO();

		/**
		* @return the pointer to the shape's colors' Vertex Buffer Object.
		*/
		GLuint* getColorsVBO();

		/**
		* @return the shape's vertices.
		*/
		vector<Vertex> getVertices();

		/**
		* @return the shape's model matrix.
		*/
		mat4 getModel();

		/**
		* Changes the model matrix.
		*
		* @param model - new model matrix.
		*/
		void setModel(mat4 model);

		/**
		* @return a pair containing the coordinates of the shape's mid point.
		*/
		pair<float, float> getPosition();

		/**
		* Moves the shape.
		* 
		* @param x - horizontal movement.
		* @param y - vertical movement.
		*/
		void move(float x, float y);

		/**
		* @return the rotation angle of the shape in degrees.
		*/
		float getRotationAngle();

		/**
		* Rotates the shape.
		* 
		* @param angle - angle (in degrees) to add to the shape's rotation angle.
		*/
		void rotate(float angle);
};