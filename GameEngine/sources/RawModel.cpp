#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ObjectLoader.h"
#include "RawModel.h"
#include <vector>


void RawModel::load(bool fromObjFormat){
	
	objectLoader.load(fromObjFormat);
	vboVector = objectLoader.getVBO();
	eboVector = objectLoader.getEBO();
	genBuffers();

}

void RawModel::genBuffers(){
	if(eboVector != nullptr){
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (eboVector->size())*sizeof(GLuint) , &((*eboVector)[0]), GL_STATIC_DRAW);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (vboVector->size())*sizeof(GLfloat), &((*vboVector)[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}	else {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (vboVector->size())*sizeof(GLfloat), &((*vboVector)[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void RawModel::draw(){
	if(eboVector != nullptr){
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, eboVector->size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	} else {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (vboVector->size())/5);
		glBindVertexArray(0); 
	}
}