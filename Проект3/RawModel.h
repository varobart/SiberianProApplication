#ifndef RAW_MODEL_H
#define RAW_MODEL_H


#include <GLFW/glfw3.h>
#include "ObjectLoader.h"
#include <vector>

class RawModel{

public:
	RawModel(const char* fileName): objectLoader(ObjectLoader(fileName)){};
	void load(bool fromObjFormat = true);
	inline GLuint getVAO(){return VAO;}
	inline std::vector<GLuint>* getEBOvector(){return eboVector;}
	void draw();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	ObjectLoader objectLoader;
	std::vector<GLuint>* eboVector;
	std::vector<GLfloat>* vboVector;

	void genBuffers();

};



#endif