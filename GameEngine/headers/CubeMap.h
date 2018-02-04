#ifndef CUBE_MAP_H
#define CUBE_MAP_H


#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <SOIL2.h>
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

class CubeMap{

public:
	CubeMap(const char* fileWithCoords, const char* rightFace, const char* leftFace, const char* upperFace, const char* downFace, 
		const char* backFace, const char* frontFace);
	void load();
	void draw();

private:
	GLuint textureID;
	GLuint VAO;
	GLuint VBO;
	const char* fileWithCoords;
	std::vector<GLfloat>* vboVector;
	std::vector<std::string> faces;

	void loadCubeMap();
	void loadVBO();
	void genBuffers();

};







#endif
