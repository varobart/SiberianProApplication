

#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include<string>
#include<vector>
#include <GLFW/glfw3.h>

class ObjectLoader{

public:
	ObjectLoader(){};
	ObjectLoader(const char* fileName);
	ObjectLoader(const ObjectLoader& objectLoader);
	ObjectLoader(ObjectLoader&& objectLoader);
	std::vector<GLfloat>* getVBO();
	std::vector<GLuint>* getEBO();
	void load(bool fromObjFormat = true);


private:
	std::vector<GLfloat>* VBO;
	std::vector<GLuint>* EBO;
	const char* fileName;

	void loadFromTxt();
	void loadFromObj();
};



#endif