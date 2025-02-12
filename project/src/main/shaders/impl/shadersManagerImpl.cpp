#include "../api/shadersManager.hpp"
#include "../../utils.hpp"
#include <direct.h>
#include <string>

ShadersManager::ShadersManager(char* vertexFile, char* fragmentFile) {
	this->programId = 0;
	this->vertexFile = vertexFile;
	this->fragmentFile = fragmentFile;
}

char* ShadersManager::readShaderSource(char* shaderFile) {
	char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) == nullptr) {
        fprintf(stderr, "ERROR - LOADING CURRENT DIRECTORY FAILED\n");
        exit(-1);
    }
    strcat_s(cwd, sizeof(cwd), shaderFile);
    FILE* file;
    errno_t err = fopen_s(&file, cwd, "rb");
    if (err != 0 || file == nullptr) {
        fprintf(stderr, "ERROR %d - FILE LOADING FAILED (%s)\n", err, cwd);
        exit(-1);
    }
    if (fseek(file, 0L, SEEK_END) != 0) {
        fprintf(stderr, "ERROR - SEEKING END OF FILE FAILED (%s)\n", cwd);
        fclose(file);
        exit(-1);
    }
    long size = ftell(file);
    if (size == -1L) {
        fprintf(stderr, "ERROR - FILE SIZE (%s)\n", cwd);
        fclose(file);
        exit(-1);
    }
    if (fseek(file, 0L, SEEK_SET) != 0) {
        fprintf(stderr, "ERROR - SEEKING START OF FILE FAILED (%s)\n", cwd);
        fclose(file);
        exit(-1);
    }
    char* buf = new(std::nothrow) char[size + 1];
    if (buf == nullptr) {
        fprintf(stderr, "ERROR - ALLOCATING FRAME BUFFER FAILED (%s)\n", cwd);
        fclose(file);
        exit(-1);
    }
    size_t bytesRead = fread(buf, 1, size, file);
    if (bytesRead != size) {
        fprintf(stderr, "ERROR - READING FILE FAILED (%s)\n", cwd);
        delete[] buf;
        fclose(file);
        exit(-1);
    }
    buf[size] = '\0';
    fclose(file);
    return buf;
}

void ShadersManager::createProgram() {
	int success;
	char infoLog[512];
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLchar* vertexShader = this->readShaderSource(vertexFile);
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		fprintf(stderr, "ERROR - VERTEX SHADER COMPILATION FAILED\n %s \n", infoLog);
	}

	GLchar* fragmentShader = this->readShaderSource(fragmentFile);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		fprintf(stderr, "ERROR - FRAGMENT SHADER COMPILATION FAILED\n %s \n", infoLog);
	}

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	this->programId = programId;
}

GLuint ShadersManager::getProgramId() const {
	return this->programId;
}