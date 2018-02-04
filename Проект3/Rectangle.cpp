#include"Rectangle.h"
#include<fstream>
#include<iostream>
#include<sstream>

Rectangle::Rectangle(const char* fileName, const char* objName)
{
	std::ifstream file(fileName, std::fstream::in);
	std::stringstream strStream;
	strStream << file.rdbuf();
	std::string s;
	strStream >> s;
	if(!s.compare(objName)){
		strStream >> xMin;
		strStream >> yMin;
		strStream >> zMin;
		strStream >> xMax;
		strStream >> yMax;
		strStream >> zMax;
	}

};