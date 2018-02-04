#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>


class Window{

public:
	Window(GLuint width, GLuint height) : width(width), height(height){};
	int init();
	inline GLFWwindow* getGLFWwindow(){
		return window;
	}
	inline int getWidth(){
		return width;
	}
	inline int getHeight(){
		return height;
	}
	int shouldClose();


private:
	GLuint width;
	GLuint height;
	GLFWwindow* window;





};




#endif