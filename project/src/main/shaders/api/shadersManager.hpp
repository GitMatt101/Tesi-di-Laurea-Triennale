#pragma once
#include <gl/glew.h>
#include "../../utils.hpp"

class ShadersManager {
	private:
		GLuint programId;
		char* vertexFile;
		char* fragmentFile;

		/**
		* Reads a shader file.
		* 
		* @param shaderFile - shader file to read.
		* @return the shader file turned into a string of characters.
		*/
		char* readShaderSource(char* shaderFile);

	public:
		/**
		* @param vertexFile - name of the vertex shader file.
		* @param fragmentFile - name of the fragment shader file.
		*/
		ShadersManager(char* vertexFile, char* fragmentFile);

		/**
		* Creates the program's id for the application.
		*/
		void createProgram();

		/**
		* Retrieves the program's id.
		*/
		GLuint getProgramId() const;
};