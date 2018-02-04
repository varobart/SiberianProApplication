

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include<string>

class Rectangle{

public:
	Rectangle(){};
	Rectangle(const char* fileName, const char* objName);
	//Rectangle(const Rectangle&);
	inline float getxMax() const {return xMax;};
	inline float getyMax() const {return yMax;};
	inline float getzMax() const {return zMax;};
	inline float getxMin() const {return xMin;};
	inline float getyMin() const {return yMin;};
	inline float getzMin() const {return zMin;};


private:
	float xMin;
	float yMin;
	float zMin;
	float xMax;
	float yMax;
	float zMax;



};



#endif