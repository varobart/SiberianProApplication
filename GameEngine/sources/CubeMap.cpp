#include "CubeMap.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// GLFW
#include <GLFW/glfw3.h>

CubeMap::CubeMap(const char* fileWithCoords, const char* rightFace, const char* leftFace, const char* upperFace, const char* downFace, 
				 const char* backFace, const char* frontFace) : fileWithCoords(fileWithCoords){
					faces.push_back(rightFace);
					faces.push_back(leftFace);
					faces.push_back(upperFace);
					faces.push_back(downFace);
					faces.push_back(backFace);
					faces.push_back(frontFace);
				}



void CubeMap::loadVBO(){

	std::ifstream inFile;
	inFile.open(fileWithCoords);

	if(inFile.fail()){
		std::cerr << "Error opening file" << std::endl;
		exit(1);
	}

	std::stringstream data;

	data << inFile.rdbuf();

	std::string line;

	vboVector = new std::vector<GLfloat>();


	float x, y, z;

	while(std::getline(data, line)){
	
		sscanf(line.c_str(), "%f, %f, %f,", &x, &y, &z);

		std::cout << x << "  " << y << "  " << z << "  " << '\n';

		vboVector-> push_back(x);
		vboVector -> push_back(y);
		vboVector-> push_back(z);

	}


}


void CubeMap::loadCubeMap(){

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (image)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            SOIL_free_image_data(image);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            SOIL_free_image_data(image);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void CubeMap::genBuffers(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (vboVector->size())*sizeof(GLfloat), &((*vboVector)[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void CubeMap::load(){

	loadVBO();
	loadCubeMap();
	genBuffers();
}




void CubeMap::draw(){
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glDrawArrays(GL_TRIANGLES, 0, (vboVector->size())/3);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

}