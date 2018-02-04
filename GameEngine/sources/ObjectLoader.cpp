#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include "ObjectLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;


ObjectLoader::ObjectLoader(const char* fileName) : fileName(fileName), VBO(nullptr), EBO(nullptr){}




void ObjectLoader::load(bool fromObjFormat){

	if(!fromObjFormat){
		loadFromTxt();
	} else {
		loadFromObj();
	}


}



void ObjectLoader::loadFromObj(){

	vector<glm::vec3> vertices;
	vector<glm::vec2> textureCoords;
	vector<glm::vec3> normals;


	ifstream inFile;
	inFile.open(fileName);

	if(inFile.fail()){
		cerr << "Error opening file" << endl;
		exit(1);
	}

	while(inFile.good()){
		string* currentLine = new string();

		getline(inFile, *currentLine);

		//currentLine = new std::string(temp);

		if((*currentLine)[0] != 'f'){

		
			//cout << *currentLine << endl;

			if((*currentLine)[0]  == 'v' && (*currentLine)[1]  == ' '){
				float vertex[3];
				sscanf(currentLine->c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
				//istringstream istrstream = istringstream(currentLine[2]);
				//for(int i = 0; i < 3 ;i++){
				//	istrstream >> vertex[i];
				//}

				//glm::vec3 vertex = glm::vec3(vertex[0], vertex[1], vertex[2]);
				vertices.push_back(glm::vec3(vertex[0], vertex[1], vertex[2]));
			}


			if((*currentLine)[0]  == 'v' && (*currentLine)[1]  == 't'){
				float texCoord[2];
				sscanf(currentLine->c_str(), "vt %f %f", &texCoord[0], &texCoord[1]);

				//glm::vec3 vertex = glm::vec3(vertex[0], vertex[1], vertex[2]);
				textureCoords.push_back(glm::vec2(texCoord[0], texCoord[1]));
			}

			if((*currentLine)[0]  == 'v' && (*currentLine)[1]  == 'n'){
				//cout << *currentLine << '\n';
				float normal[3];
				sscanf(currentLine->c_str(), "vn %f %f %f", &normal[0], &normal[1], &normal[2]);

				//glm::vec3 vertex = glm::vec3(vertex[0], vertex[1], vertex[2]);
				normals.push_back(glm::vec3(normal[0], normal[1], normal[2]));
			}

			//inFile >> currentLine;
		} 


		if((*currentLine)[0]  == 'f' && !inFile.eof()){
			vector<vector<GLuint>> face;
			if(VBO == nullptr){
				VBO = new vector<GLfloat>(3*vertices.size() + 2*textureCoords.size() + 3*normals.size());
				EBO = new vector<GLuint>(vertices.size());
			}
			int v1,t1,n1,v2,t2,n2,v3,t3,n3;
			sscanf(currentLine->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
			vector<GLuint> ver1;
			ver1.push_back(v1);
			ver1.push_back(t1);
			ver1.push_back(n1);
			vector<GLuint> ver2;
			ver1.push_back(v2);
			ver1.push_back(t2);
			ver1.push_back(n2);
			vector<GLuint> ver3;
			ver1.push_back(v3);
			ver1.push_back(t3);
			ver1.push_back(n3);
			face.push_back(ver1);
			face.push_back(ver2);
			face.push_back(ver3);


			for(int i = 0; i < 3 ;i++)
			{
				//cout  << face[0][3*i] << endl;
				EBO->push_back(face[0][3*i]);
				glm::vec3 vertex = vertices.at(face[0][3*i] - 1);
				glm::vec2 texCoord = textureCoords.at(face[0][3*i + 1] - 1);
				glm::vec3 normal = normals.at(face[0][3*i + 2] - 1);
				VBO->at(8*(face[0][3*i] - 1) + 0) = vertex.x;
				VBO->at(8*(face[0][3*i]-1) +1) = vertex.y;
				VBO->at(8*(face[0][3*i]-1) + 2 ) = vertex.z;
				VBO->at(8*(face[0][3*i]-1) + 3) = texCoord.x;
				VBO->at(8*(face[0][3*i]-1) + 4 ) = texCoord.y;
				VBO->at(8*(face[0][3*i]-1) + 5 ) = normal.x;
				VBO->at(8*(face[0][3*i]-1) + 6 ) = normal.y;
				VBO->at(8*(face[0][3*i]-1) + 7 ) = normal.z;
				//cout << 8*(face[0][3*i] - 1) << '\n';
				//vb0oVector.push_back(normal.x);
				//vboVector.push_back(normal.y);
				//vboVector.push_back(normal.z);
			}

		
		}
	}


}



void ObjectLoader::loadFromTxt(){

	ifstream inFile;
	inFile.open(fileName);

	if(inFile.fail()){
		cerr << "Error opening file" << endl;
		exit(1);
	}

	std::stringstream data;

	data << inFile.rdbuf();

	std::string line;

	VBO = new std::vector<GLfloat>();


	float x, y, z;
	float xTexture, yTexture;

	while(std::getline(data, line)){
	
		sscanf(line.c_str(), "%f, %f, %f, %f, %f,", &x, &y, &z, &xTexture, &yTexture);

		cout << x << "  " << y << "  " << z << "  " << xTexture << "  " << yTexture << "  " << '\n';

		VBO -> push_back(x);
		VBO -> push_back(y);
		VBO -> push_back(z);
		VBO -> push_back(xTexture);
		VBO -> push_back(yTexture);
	}


}



std::vector<GLfloat>* ObjectLoader::getVBO(){
	return VBO;
}

std::vector<GLuint>* ObjectLoader::getEBO(){
	return EBO;
}