#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL2.h>

// GLFW


#include "Shader.h"
#include "Rectangle.h"
#include "Window.h"
#include <GLFW/glfw3.h>
class Collisions{

public:
	static bool intersectRayParallelepiped(Rectangle& rect, glm::vec3& cameraPos, Window& window,
								glm::mat4& modelMatrix, std::vector<glm::vec3>& normals,
								int xPos,int yPos, const glm::mat4& projMatrix, const glm::mat4& viewMatrix);



private:
	static glm::vec3 transformMousePosToWorld(int xPos,int yPos, const glm::mat4& projMatrix, const glm::mat4& viewMatrix, Window& window );





};




#endif